#pragma once

// C
#include <cstdint>
#include <cstdio>
#include <ctime>

// Log severity level
enum class LogLevel : uint8_t { None = 0, Info, Warn, Error, Fatal };

// Module unique ID to know which module caused a log
enum class LogModule : uint8_t { None = 0, Sensor, Buffer, Uploader, Modem, Power, System };

constexpr size_t LogBufferCapacity{256};

// Event codes
namespace Event {

constexpr uint8_t Boot{0};
constexpr uint8_t ShutDown{1};
constexpr uint8_t ReadOk{2};
constexpr uint8_t ReadFail{3};
constexpr uint8_t BufferFull{4};
constexpr uint8_t BufferEightyPercent{5};
constexpr uint8_t UploadOk{6};
constexpr uint8_t Uploadretry{7};
constexpr uint8_t ModemAttachFail{8};
constexpr uint8_t ModemTimeout{9};
constexpr uint8_t SleepEnter{10};
constexpr uint8_t SleepExit{11};

} // namespace Event

// Compact structured log entry stored in just 11 bytes
#pragma pack(push, 1)
struct LogEntry {
  uint32_t timestamp{0}; // Unix timestamp
  LogLevel level{LogLevel::None}; // Log level
  LogModule module{LogModule::None}; // From which module log came
  uint8_t event_code{0}; // Event code of the log
  uint32_t payload{0}; // Optional payload, for example retry count, error code or smth else
};
#pragma pack(pop)

namespace Logger {
void init();

// Stores binary entry and logs to console
void log(LogLevel level, LogModule module, uint8_t event_code, uint32_t playload = 0);

// Get logs that are saved, returns how many logs have been saved
size_t get_logs(LogEntry* out, size_t max_count);

// clear all logs
void clear();

// How many logs are saved
size_t count();
} // namespace Logger

// Macros to easier call log functions
#define log_info(mod, event, ...) Logger::log(LogLevel::Info, mod, event, ##__VA_ARGS__);
#define log_warn(mod, event, ...) Logger::log(LogLevel::Warn, mod, event, ##__VA_ARGS__);
#define log_error(mod, event, ...) Logger::log(LogLevel::Error, mod, event, ##__VA_ARGS__);
#define log_fatal(mod, event, ...) Logger::log(LogLevel::Fatal, mod, event, ##__VA_ARGS__);