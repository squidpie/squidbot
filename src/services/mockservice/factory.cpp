#include "mockservice.h"

// Factory
extern "C" void create(std::string lib_path, std::shared_ptr<Context> context) {
  PLOGD << "MockService_t create" << std::endl;
  auto service_context = std::dynamic_pointer_cast<CoreContext>(context);
  auto service = std::make_shared<MockService_t>(service_context);
  service_context->service_manager->register_service<MockService>(
      lib_path, std::dynamic_pointer_cast<ServiceBase>(service));
  service->start();
}