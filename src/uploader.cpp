// C
#include <cstdlib>

// Developoer
#include "uploader.h"
#include "logger.h"

Uploader::Uploader(RingBuffer& ring_buffer) : ring_buffer_(ring_buffer) {}

bool Uploader::uploadBatch() {
  if (ring_buffer_.count() == 0) {
    return true;
  }

  if (!modemPowerOn()) {
    log_error(LogModule::Modem, Event::ModemAttachFail);
    return false;
  }

  if (!modemConnect()) {
    log_error(LogModule::Modem, Event::ModemAttachFail);
    modemPowerOff();
    return false;
  }

  // Copy data for for upload
  SensorData batch[BufferCapacity];
  size_t count = ring_buffer_.getAll(batch, BufferCapacity);

  // Retry with exponential backoff
  for (retry_count_ = 0; retry_count_ < MaxUploadRetries; retry_count_++) {
    if (mqttPublish(batch, count)) {
      log_info(LogModule::Uploader, Event::UploadOk, count);
      ring_buffer_.clear();
      modemPowerOff();
      return true;
    }

    // Backoff: 5s, 10s, 20s
    uint32_t backoff = BaseBackoffSec * (1 << retry_count_);
    log_warn(LogModule::Uploader, Event::UploadRetry, retry_count_ + 1);
    // On real device sleep for backoff time
  }

  // All reties failed, keep data and try next cycle
  log_error(LogModule::Uploader, Event::UploadFail);
  return false;
}

// Simulated modem operations
bool Uploader::modemPowerOn() {
  if (std::rand() % 10 == 0) {
    return false;
  }
  return true;
}

void Uploader::modemPowerOff() {
  // modem powering off logic in real system
}

bool Uploader::modemConnect() {
  if (std::rand() % 10 == 0) {
    return false;
  }
  return true;
}

bool Uploader::mqttPublish(SensorData* data, size_t count) {
  // Connect to MQTT broker over TLS, publish to topic
  //
  // Protocol choice: MQTT
  // - Low overhead, ideal for constrained devices
  // - Built-in QoS levels for delivery guarantees
  // - Persistent sessions handle intermittent connectivity
  //
  // Topic: devices/{device_id}/sensor_data

  if (std::rand() % 5 == 0) {
    return false;
  }

  return true;
}