#pragma once

#include <cstdint>

class TemperatureSensor {
 public:
  TemperatureSensor();
  void init();
  float readTemperature();

 private:
  float temperature_{10.7f};
};