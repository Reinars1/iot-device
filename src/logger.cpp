// C
#include <cstring>
#include <cstdio>
#include <ctime>

// Developer
#include "logger.h"

// Buffer which is saved somewhere in RTC or some other non-volatile memory
static LogEntry log_ring_[LogBufferCapacity];
static size_t log_head_ = 0;
static size_t log_count_ = 0;

namespace Logger {

void log(LogLevel level, LogModule module, uint8_t event_code, uint32_t payload) {
  LogEntry entry;
  entry.timestamp = static_cast<uint32_t>(std::time(nullptr));
  entry.level = level;
  entry.module = module;
  entry.event_code = event_code;
  entry.payload = payload;

  log_ring_[log_head_] = entry;
  log_head_ = (log_head_ + 1) % LogBufferCapacity;
  if (log_count_ < LogBufferCapacity) {
    log_count_++;
  }
  // To see in terminal what is being logged
  printf("Timestamp: %u, level: %u, module: %u, event code: %u, payload: %u\n", entry.timestamp,
         static_cast<uint8_t>(entry.level), static_cast<uint8_t>(entry.module), entry.event_code, entry.payload);
}

bool getLogs(LogEntry* out, size_t max_count) {
  if (out == nullptr || log_count_ == 0) {
    return false;
  }

  size_t to_copy = (max_count < log_count_) ? max_count : log_count_;

  // Find the oldest entry in the ring
  size_t start;
  if (log_count_ < LogBufferCapacity) {
    start = 0;
  } else {
    start = log_head_; // Head points to the oldest when full
  }

  for (size_t i = 0; i < to_copy; i++) {
    size_t idx = (start + i) % LogBufferCapacity;
    out[i] = log_ring_[idx];
  }

  return true;
}

void clear() {
  log_head_ = 0;
  log_count_ = 0;
}
size_t count() {
  return log_count_;
}
} // namespace Logger