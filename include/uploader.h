#pragma once

// C
#include <cstdint>

// Developer
#include "ring_buffer.h"

class Uploader {
 public:
  Uploader(RingBuffer& ring_buffer);

  // Uploads all data readings
  // Returns status if upload was successfull
  bool uploadBatch();

 private:
  bool modemPowerOn();
  void modemPowerOff();
  bool modemConnect();
  bool mqttPublish(SensorData* data, size_t count);

  static constexpr uint8_t MaxUploadRetries{3};
  static constexpr uint32_t BaseBackoffSec{5};

  uint8_t retry_count_{0};

  RingBuffer& ring_buffer_;
};
