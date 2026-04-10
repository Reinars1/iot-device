// C
#include <ctime>
#include <cstdlib>

// Developer
#include "logger.h"
#include "sensor.h"
#include "ring_buffer.h"
#include "uploader.h"
#include "power.h"

// Configuration
constexpr uint32_t SensorIntervalSec{5}; // 5min in seconds
constexpr uint32_t UploadIntervalsec{30}; // 30min in seconds
constexpr uint32_t ReadingsPerUpload{UploadIntervalsec / SensorIntervalSec}; // 6

// In production these would be RTOS tasks created with
// xTaskCreate (FreeRTOS)
// Each task would block on its own timer or event.

Sensor sensor;
RingBuffer ring_buffer;
Uploader uploader(ring_buffer);

int main() {
  std::srand(std::time(nullptr));

  sensor.init();
  log_info(LogModule::System, Event::Boot);

  uint32_t readings_since_upload = 0;

  while (true) {
    // --- Sensor Task ---
    // On real RTOS: separate thread, wakes on 5-min timer
    Power::onWake();
    SensorData reading = sensor.read();
    ring_buffer.push(reading);
    readings_since_upload++;

    // --- Upload Task ---
    // On real RTOS: separate thread, wakes on 30-min timer
    // or when buffer signals high-water mark
    if (readings_since_upload >= ReadingsPerUpload || ring_buffer.UploadThresholdReached()) {
      if (uploader.uploadBatch()) {
        readings_since_upload = 0;
      }
      // If upload fails, data stays in buffer — try next cycle
    }

    // --- Power Task ---
    // On real device: deep sleep until next RTC alarm
    Power::sleep(SensorIntervalSec);
  }
  return 0;
}