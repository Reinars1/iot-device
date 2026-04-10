# Air Quality Monitor

Battery-powered cellular IoT device that reads air temperature and humidity every 5 minutes, buffers measurements locally, and uploads data to a cloud backend over LTE-M every 30 minutes.

## Platform

- **Language:** C++17
- **RTOS:** None — structured as if running on a small RTOS (tasks, queues, timers) but implemented as a simulated task loop for portability and ease of review
- **Board/target:** architecture designed for ARM Cortex-M4 + LTE-M modem
- **Build system:** CMake 3.20+
- **Compiler:** GCC 13+

## Project structure

```
air-quality-monitor/
├── CMakeLists.txt
├── README.md
├── DESIGN.md
├── include/
│   ├── sensor.h
│   ├── temperature_sensor.h
│   ├── humidity_sensor.h
│   ├── ring_buffer.h
│   ├── uploader.h
│   ├── logger.h
│   └── power.h
└── src/
    ├── main.cpp
    ├── sensor.cpp
    ├── temperature_sensor.cpp
    ├── humidity_sensor.cpp
    ├── ring_buffer.cpp
    ├── uploader.cpp
    ├── logger.cpp
    └── power.cpp
```

## Build and run

```bash
mkdir build
cd build
cmake ..
make
./air-quality-monitor
```

## Architecture

The firmware is organised into simulated tasks that mirror how a real RTOS-based system would operate:

- **Sensor task** — Wakes every 5 minutes, reads temperature and humidity, retries up to 3 times on failure, timestamps the reading, and pushes it into the ring buffer.
- **Upload task** — Uploads every 30 minutes or when the ring buffer hits 80%. Powers on the modem, connects to the network, publishes buffered readings via MQTT, and powers the modem off. Retries with exponential backoff on failure. If all retries fail, data stays in the buffer for the next cycle.
- **Power task** — Puts the device into deep sleep between sensor reads. Modem is only powered during upload windows.

## What is simulated

- **Sensor readings** — Return values with ~10% failure rate per sensor
- **Modem/network** — interface with simulated connection failures
- **MQTT publish** — Logs and simulates occasional failures
- **Deep sleep** — Simulated with `std::this_thread::sleep_for`
- **Flash storage** — Log and buffer persistence described in DESIGN.md, not implemented in simulation

## What is real

- Ring buffer with overwrite-oldest policy and configurable UploadThreshold trigger
- Independent sensor retry logic with per-attempt warning logs and final error on exhaustion
- Upload retry with exponential backoff (5s, 10s, 20s)
- Structured binary logging (11-byte entries with severity, module, event code, payload(optional))
- Graceful handling of all failure modes — sensor read failures, modem attach failures, upload timeouts — no crashes or lockups

## Buffer-full policy

When the ring buffer is full, the oldest reading is overwritten, newer data is more valuable than stale data. UploadThreshold trigger (80%) doesnt allow buffer to ever overflow. See DESIGN.md for further discussion.

## Design document

See [DESIGN.md](DESIGN.md) for protocol choice, power strategy, CRA security compliance, troubleshooting design, and planned improvements.