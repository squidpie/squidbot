#include "squidbot.h"

int main() {
  plog::init(plog::debug, "squidbot.log"); // make this configurable

  PLOGD << "Starting OBS Service";
  ServiceManager service_manager;
  ServiceContainer obs = ServiceContainer("obs");
  service_manager.load(&obs);
  obs.run();
  PLOGD << "OBS Service Started";
}