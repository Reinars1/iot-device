#pragma once

// C
#include <cstdint>
#include <cstddef>

// Developer
#include "sensor.h"

static constexpr size_t BufferCapacity{72}; // 12 readings/hour. 6h

class RingBuffer {
 public:
  RingBuffer();

  void push(SensorData& reading);

  size_t count();
  bool isFull();
  bool EightyPercentReached();
  void clear();

  // Get buffer for upload without deleting data
  size_t getAll(SensorData* out, size_t max_count);

 private:
  static constexpr uint8_t EightPercent{80};

  SensorData buffer_[BufferCapacity];
  size_t head_{0};
  size_t tail_{0};
  size_t count_{0};
};