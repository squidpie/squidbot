#include "mockservice.h"


void MockServiceRunAction::run_action() {
  Event rx = event_client->receive();
  if (rx != NULL_EVENT) event_client->send(rx);
}

// Factory
extern "C++" std::shared_ptr<ServiceBase> create_service(ServiceContext context) {
  return std::make_shared<MockService_t>(context);
}

extern "C" void destroy_service(ServiceBase *p) {
  delete dynamic_cast<MockService_t*>(p);
}