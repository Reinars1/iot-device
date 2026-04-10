// C
#include <chrono>
#include <thread>

// Developer
#include "logger.h"
#include "power.h"

namespace Power {
void sleep(uint32_t seconds) {
  log_info(LogModule::Power, Event::SleepEnter);

  // On real device:
  // 1. Save state to RTC memory (buffer index, fail counters)
  // 2. Configure RTC alarm for wakeup
  // 3. Disable peripherals (sensor, modem)
  //
  // In simulation: just wait

  std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void onWake() {
  log_info(LogModule::Power, Event::SleepExit);

  // On real device:
  // 1. Reinitialise peripherals (GPIO, I2C, SPI)
  // 2. Restore state from RTC memory
}
} // namespace Power