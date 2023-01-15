#include <typeindex>

#include "service.h"
#include "service_manager.h"
#include "utils/lib_loader.h"

std::shared_ptr<ServiceInterfaceBase>
ServiceManager::get_service_interface(std::type_index index) {
  return services->at(index).second->get_interface();
}

void ServiceManager::load(std::shared_ptr<CoreContext> context) {
  libloader = std::make_unique<LibLoader<ServiceLoader>>(context);
}

void ServiceManager::load_service(std::string lib_name) {
  libloader->load_lib(lib_name);
}

void ServiceManager::_register_service(
    const std::type_index index,
    std::pair<std::string, std::shared_ptr<ServiceBase>> entry) {
  services->insert({index, entry});
}

bool ServiceManager::is_service_available(std::type_index index) {
  return services->find(index) != services->end();
}

void ServiceManager::unload() {
  services->clear();
  libloader.reset();
}

void ServiceManager::_unload(std::type_index index) {
  services->at(index).second->stop();
  services->erase(index);
}

void ServiceManager::_reload(std::type_index index) {
  auto lib_path = services->at(index).first;
  _unload(index);
  libloader->load_lib(lib_path);
}