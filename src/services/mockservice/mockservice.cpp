#include "mockservice.h"

void MockServiceRunAction::run_action() {
  Event rx = event_client->receive();
  if (rx != NULL_EVENT) event_client->send(rx);
}

// Factory
extern "C" void create(std::shared_ptr<Context> context) {
  std::cerr << "DEBUG OUTPUT :: MockService_t create" << std::endl;
  auto service_context = std::dynamic_pointer_cast<ServiceContext>(context);
  auto service = std::make_shared<MockService_t>(service_context);
  std::cerr << "DEBUG OUTPUT :: MockService_t reg" << std::endl;
  auto cast_service = std::dynamic_pointer_cast<ServiceBase>(service);
  assert(cast_service != __null);
  service_context->service_manager->register_service(std::type_index(typeid(MockService)), cast_service);
  std::cerr << "DEBUG OUTPUT :: MockService_t start" << std::endl;
  service->start();
  std::cerr << "DEBUG OUTPUT :: MockService_t construct complete" << std::endl;
}

extern "C" void destroy(std::unique_ptr<ServiceBase> service) {
  service->stop();
  service.reset();
}