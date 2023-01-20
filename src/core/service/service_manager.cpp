/*
service_manager.cpp
Copyright (C) 2023  Squidpie
 */

#include <condition_variable>
#include <mutex>
#include <typeindex>

#include "lib/core_version.h"
#include "pluginmanager.h"
#include "service.h"
#include "service_manager.h"
#include "utils/lib_loader.h"

std::shared_ptr<ServiceInterfaceBase>
ServiceManager::_get_interface(std::type_index index) {
  if (!is_service_available(index)) {
    PLOGE << "No Interface found";
    return nullptr;
  }
  auto service = services->at(index).second;
  return service->get_interface();
};

void ServiceManager::load(std::shared_ptr<CoreContext> _context) {
  context = _context;
  libloader = std::make_unique<LibLoader<ServiceLoader>>(_context);
}

void ServiceManager::load_service(std::string lib_name) {
  libloader->load_lib(lib_name);
}

void ServiceManager::_register_service(
    const std::type_index index,
    std::pair<std::string, std::shared_ptr<ServiceBase>> entry) {
  auto service = entry.second;
  if (service->core_version() != CORE_VERSION) {
    // LOG ERROR
    return;
  }
  services->insert({index, entry});
}

bool ServiceManager::is_service_available(std::type_index index) {
  return services->find(index) != services->end();
}

void ServiceManager::unload() {
  services->clear();
  libloader.reset();
  context.reset();
}

void ServiceManager::_unload(std::type_index index) {
  context->plugin_manager->service_unload_notify(index);
  services->at(index).second->stop();
  services->erase(index);
}

void ServiceManager::_reload(std::shared_ptr<std::type_index> index) {
  std::shared_ptr<std::mutex> plugin_mutex = std::make_shared<std::mutex>();
  
  plugin_mutex->lock();
  auto plugin_thread =
      context->plugin_manager->service_reload_warn(plugin_mutex, index);
  auto lib_path = services->at(*index).first;
  services->at(*index).second->stop();
  services->erase(*index);
  libloader->load_lib(lib_path);
  plugin_mutex->unlock();
  plugin_thread.join();
}