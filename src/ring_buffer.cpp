#include "ring_buffer.h"
#include "logger.h"

RingBuffer::RingBuffer() {}

void RingBuffer::push(SensorData& reading) {
  if (isFull()) {
    // If buffer is full override oldest, fresh data better than old data
    tail_ = (tail_ + 1) % BufferCapacity;
    count_--;
    log_warn(LogModule::Buffer, Event::BufferFull);
  }

  buffer_[head_] = reading;
  head_ = (head_ + 1) % BufferCapacity;
  count_++;

  if (EightyPercentReached()) {
    log_warn(LogModule::Buffer, Event::BufferEightyPercent);
  }
}

size_t RingBuffer::count() {
  return count_;
}

bool RingBuffer::isFull() {
  return count_ == BufferCapacity;
}

bool RingBuffer::EightyPercentReached() {
  return count_ >= (BufferCapacity * EightPercent / 100);
}

void RingBuffer::clear() {
  head_ = 0;
  tail_ = 0;
  count_ = 0;
}

void RingBuffer::getAll(SensorData* out, size_t max_count) {
  if (out == nullptr || count == 0) {
    log_warn(LogModule::Buffer, Event::ReadFail);
    return;
  }
}