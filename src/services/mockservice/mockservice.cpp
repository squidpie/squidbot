#include "mockservice.h"

void MockServicePluginInterface::test()  {
  static uint test = 0;
  PLOGD << "TEST OUTPUT :: MockService test() " << test++;
}

void MockServiceRunAction::run_action() {
  Event rx = event_client->receive();
  if (rx != NULL_EVENT) event_client->send(rx);
}

extern "C" void destroy(std::unique_ptr<ServiceBase> service) {
  service->stop();
  service.reset();
}