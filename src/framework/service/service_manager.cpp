#include "service.h"
#include "service_manager.h"

std::shared_ptr<ServiceInterfaceBase> ServiceManager::get_service_interface(std::type_index index) {
  std::cerr << "DEBUG OUTPUT :: about to step into services" << std::endl;
  return services[index]->get_interface();
}

void ServiceManager::load(std::shared_ptr<ServiceContext> context) {
  std::make_unique<LibLoader<ServiceLoader>>(context);
}