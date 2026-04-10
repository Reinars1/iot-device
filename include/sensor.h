#pragma once

// C
#include <cstdint>

// Developer
#include "humidity_sensor.h"
#include "temperature_sensor.h"

#pragma pack(push, 1)
struct SensorData {
  uint32_t timestamp{0};
  float temperature{0.0f};
  float humidity{0.0f};
  bool temp_valid{false};
  bool hum_valid{false};
};
#pragma pack(pop)

class Sensor {
 public:
  Sensor();
  void init();
  SensorData read();

 private:
  void readWithRetryTemp(SensorData& data);
  void readWithRetryHum(SensorData& data);

  HumiditySensor humidity_sensor_;
  TemperatureSensor temperature_sensor_;

  static constexpr uint8_t MaxRetries{3};
};