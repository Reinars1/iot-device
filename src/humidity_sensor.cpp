// C
#include <cstdlib>
#include <cmath>

// Developer
#include "logger.h"
#include "humidity_sensor.h"

HumiditySensor::HumiditySensor() {}

void HumiditySensor::init() {
  // On real device configure I2C/SPI
}
float HumiditySensor::readHumidity() {
  if (std::rand() % 10 == 0) {
    log_error(LogModule::HumiditySensor, Event::ReadFail);
    return NAN;
  }
  log_error(LogModule::HumiditySensor, Event::ReadOk);
  return humidity_;
}