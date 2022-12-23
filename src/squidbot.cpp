#include "squidbot.h"

int main() {
  ServiceManager service_manager;
  ServiceContainer obs = ServiceContainer("obs");

  service_manager.load(&obs);
  obs.run();
  std::cout << "DEBUG OUTPUT :: SQUIDBOT IS RUNNING" << std::endl;
}