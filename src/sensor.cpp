// C
#include <cmath>
#include <ctime>

// Developer
#include "sensor.h"
#include "logger.h"

Sensor::Sensor() {}

void Sensor::init() {
  humidity_sensor_.init();
  temperature_sensor_.init();
}

SensorData Sensor::read() {
  SensorData sensor_data;
  sensor_data.timestamp = static_cast<uint32_t>(std::time(nullptr));

  //* Temperature reading
  readWithRetryTemp(sensor_data);
  //* Humidity reading
  readWithRetryHum(sensor_data);
  return sensor_data;
}

void Sensor::readWithRetryTemp(SensorData& data) {
  float temperature = temperature_sensor_.readTemperature();
  if (!std::isnan(temperature)) {
    data.temperature = temperature;
    data.temp_valid = true;
    log_info(LogModule::TemperatureSensor, Event::ReadOk);
    return;
  }

  log_warn(LogModule::TemperatureSensor, Event::ReadFail);

  //* Retry up to MaxRetries times
  for (size_t i = 0; i < MaxRetries; i++) {
    temperature = temperature_sensor_.readTemperature();
    if (!std::isnan(temperature)) {
      data.temperature = temperature;
      data.temp_valid = true;
      log_info(LogModule::TemperatureSensor, Event::ReadOk);
      return;
    }
    log_warn(LogModule::TemperatureSensor, Event::ReadFail, i + 1);
  }

  log_error(LogModule::TemperatureSensor, Event::ReadFail);
}

void Sensor::readWithRetryHum(SensorData& data) {
  float humidity = humidity_sensor_.readHumidity();
  if (!std::isnan(humidity)) {
    data.humidity = humidity;
    data.hum_valid = true;
    log_info(LogModule::HumiditySensor, Event::ReadOk);
    return;
  }

  log_warn(LogModule::HumiditySensor, Event::ReadFail);

  //* Retry up to MaxRetries times
  for (size_t i = 0; i < MaxRetries; i++) {
    humidity = humidity_sensor_.readHumidity();
    if (!std::isnan(humidity)) {
      data.humidity = humidity;
      data.hum_valid = true;
      log_info(LogModule::HumiditySensor, Event::ReadOk);
      return;
    }
    log_warn(LogModule::HumiditySensor, Event::ReadFail, i + 1);
  }

  log_error(LogModule::HumiditySensor, Event::ReadFail);
}
