#pragma once

// C
#include <cstdint>
#include <cstddef>

// Developer
#include "sensor.h"

class RingBuffer {
 public:
  RingBuffer();

  void push(SensorData& reading);
  bool pop(SensorData& out);

  size_t count();
  bool isFull();
  bool EightyPercentReached();
  void clear();

  // Get buffer for upload without deleting data
  void getAll(SensorData& out, size_t max_count);

 private:
  static constexpr size_t BufferCapacity{72}; // 12 readings/hour. 6h
  static constexpr uint8_t EightPercent{80};
};