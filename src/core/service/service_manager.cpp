#include <typeindex>

#include "utils/lib_loader.h"
#include "service.h"
#include "service_manager.h"

std::shared_ptr<ServiceInterfaceBase> ServiceManager::get_service_interface(std::type_index index) {
  return services[index]->get_interface();
}

void ServiceManager::load(std::shared_ptr<CoreContext> context) {
  std::make_unique<LibLoader<ServiceLoader>>(context);
}

void ServiceManager::register_service(const std::type_index index,
                      std::shared_ptr<ServiceBase> service) {
  assert(service != nullptr);
  services[index] = service;
}

bool ServiceManager::is_service_available(std::type_index index) {
  return services.find(index) != services.end();
}