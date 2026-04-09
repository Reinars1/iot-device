#include "logger.h"

int main() {
  log_info(LogModule::System, Event::Boot);
  return 0;
}