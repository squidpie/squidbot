#include "mockservice.h"

void MockServicePluginInterface::test()  {
  PLOGD << "TEST OUTPUT :: MockService test()";
}

void MockServiceRunAction::run_action() {
  Event rx = event_client->receive();
  if (rx != NULL_EVENT) event_client->send(rx);
}

// Factory
extern "C" void create(std::shared_ptr<Context> context) {
  PLOGD << "MockService_t create" << std::endl;
  auto service_context = std::dynamic_pointer_cast<ServiceContext>(context);
  auto service = std::make_shared<MockService_t>(service_context);
  service_context->service_manager->register_service(std::type_index(typeid(MockService)), std::dynamic_pointer_cast<ServiceBase>(service));
  service->start();
}

extern "C" void destroy(std::unique_ptr<ServiceBase> service) {
  service->stop();
  service.reset();
}