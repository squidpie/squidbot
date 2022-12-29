#include "service_manager.h"
#include "service.h"

void ServiceManager::init() {}

void ServiceManager::load(ServiceContainerBase* service) {
  PLOGD << "service_container address = " << service;
  service->gen_lib_path(lib_dir);
  service->init();
  service->create();
}

void ServiceManager::unload(ServiceContainerBase* service) {
  service->destroy();
}