// C
#include <cstdlib>
#include <cmath>

// Developer
#include "temperature_sensor.h"

TemperatureSensor::TemperatureSensor() {}

void TemperatureSensor::init() {
  // On real device configure I2C/SPI
}
float TemperatureSensor::readTemperature() {
  // Simultae read failure
  if (std::rand() % 10 == 0) {
    return NAN;
  }

  return temperature_;
}