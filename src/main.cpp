#include "logger.h"
#include "sensor.h"

Sensor sensor;

int main() {
  log_info(LogModule::System, Event::Boot);
  sensor.init();
  while (1) {
    sensor.read();
  }
  return 0;
}