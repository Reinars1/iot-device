# Design Document — Air Quality Monitor

---

## 1. Protocol choice: MQTT

**Why it fits this use case:**

- Low protocol overhead — MQTT PUBLISH header is ~2 bytes versus 200+ bytes for HTTP headers.
- Built-in QoS 1 guarantees the backend receives sensor data without implementing a custom acknowledgment layer. The device publishes, waits for PUBACK, and only clears the buffer on confirmation.
- Persistent sessions allow the broker to queue commands (log requests, config changes, FOTA notifications) while the device sleeps. On reconnect the device receives queued messages without polling.

**Main weakness and mitigation:**

- MQTT requires a TCP connection, which is expensive to establish over LTE-M (~5–10 seconds of radio time for TCP + TLS handshake). Mitigated by batching all buffered readings into a single publish, keeping the connection alive only for the duration of the transfer
---

## 2. Power strategy

The device spends >99% of its time in deep sleep. Three power states:

- **Deep sleep (~5 min cycles):** MCU enters lowest power mode.
- **Sensor read (every 5 min):** MCU wakes on RTC alarm, reads the sensor, stores the reading, and returns to deep sleep. Modem stays off.
- **Upload (every 30 min or on 80% UploadThreshold):** MCU wakes, powers the modem, attaches to the network,  publishes via MQTT, and powers the modem down.
**Key decisions:**

---

## 3. Security decisions (CRA compliance)

| CRA Requirement | Decision | Implementation |
|---|---|---|
| **Secure transport** | MQTT over TLS 1.2+ with server certificate verification | Most LTE-M modems  support TLS offloaded in modem firmware. The MCU configures the TLS profile — no software TLS stack on the application processor.|
| **Secrets handling** | Device credentials stored in hardware or modem secure storage | client certificates provisioned during manufacturing. Never hardcoded in firmware source or flash. Private keys are non-exportable. If a single device is compromised, its certificate can be revoked without disrupting any other devices. |
| **OTA update path** | FOTA via MQTT command topic | The backend publishes a firmware info (version, download URL, hash) to a dedicated topic. On next wake the device downloads the binary in chunks over HTTPS, verifies the hash against the info, and writes to the secondary flash partition. On reboot, the bootloader validates the new image signature before switching partitions. If validation fails or the new firmware does not report healthy within a timeout, the bootloader rolls back to the previous partition. This ensures the device is never bricked by a failed update. |

---

## 4. Troubleshooting design

**Log format:**

Each log entry is a packed 11-byte binary struct:

| Field | Type | Size | Purpose |
|---|---|---|---|
| `timestamp` | uint32_t | 4 B | Unix timestamp (seconds) |
| `level` | uint8_t | 1 B | None / Info / Warn / Error / Fatal |
| `module` | uint8_t | 1 B | TemperatureSensor / HumiditySensor / Buffer / Uploader / Modem / Power / System |
| `event_code` | uint8_t | 1 B | Numeric event ID, decoded server-side |
| `payload` | uint32_t | 4 B | Context value — retry count, error code, buffer level, etc. |

- No strings stored on device — keeps entries compact and deterministic in size.
- Human readable decoding happens on the backend.

**Persistence:**

- On a real device the log ring buffer is located in a dedicated flash sector.
- On boot the firmware scans flash to find the current head position, so logs from before a crash are preserved.
- In the simulation, persistence is not implemented — the buffer is a static array.

**How a field engineer accesses logs:**

- The backend publishes a log-request command to a dedicated MQTT topic.
- On the next upload window the device uploads its log buffer to a dedicated MQTT topic.
- The backend decodes the entries and displays them.
- No physical access to the device is required.

---

## 5. What I would do next

1. **Hardware watchdog timer.** If the main loop stalls (modem hang, I2C bus lockup, unexpected exception), a hardware watchdog resets the device.

2. **FOTA implementation with MCUboot.** The design describes the dual-bank update path but does not implement it. This is the highest-risk production feature — a failed update in the field means a truck roll.

3. **Runtime configuration over MQTT.** Allow the backend to change sensor interval, upload interval, log level, and UploadThreshold threshold without a firmware update. Configuration stored in flash, applied on next boot or immediately via a restart command.