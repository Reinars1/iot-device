#pragma once

#include <cstdint>

class HumiditySensor {
 public:
  HumiditySensor();
  void init();
  float readHumidity();

 private:
  float humidity_{5.9f};
};