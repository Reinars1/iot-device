#pragma once

#include <cstdint>

namespace Power {
// Enter low-power sleep for given duration in seconds.
// On a real device this would configure the RTC wakeup
void sleep(uint32_t seconds);

// Called on wakeup to reinitialise clocks and peripherals.
// After deep sleep most peripherals lose state.
void onWake();
} // namespace Power