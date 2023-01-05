#pragma once

#include <memory>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#include "service_loader.h"
#include "servicelib.h"
#include "utils.h"

class ServiceManagerBase {
public:
  virtual ~ServiceManagerBase() {}
  virtual void register_service(std::type_index,
                                std::shared_ptr<ServiceBase>) = 0;
};

class ServiceManager : virtual public ServiceManagerBase,
                       public std::enable_shared_from_this<ServiceManager> {
public:
  ServiceManager() {}
  ServiceManager(std::shared_ptr<ServiceContext> context) {
    // context->service_manager = shared_from_this();
    // std::make_unique<LibLoader<ServiceLoader>>(std::dynamic_pointer_cast<Context>(context));
  }
  ~ServiceManager() {}
  void load(std::shared_ptr<ServiceContext> context);
  /*
  Caller must check for NULL pointer on return
   */
  template <class T>
  std::shared_ptr<typename T::plugin_interface_t> get_interface() {
    if (services.find(std::type_index(typeid(T))) == services.end()) {
      std::cerr << "DEBUG OUTPUT :: No Interface found for type" << std::endl;
      return NULL;
    }
    return std::dynamic_pointer_cast<typename T::plugin_interface_t>(
        get_service_interface(std::type_index(typeid(T))));
  };

  void register_service(const std::type_index index,
                        std::shared_ptr<ServiceBase> service) {
    std::cerr << "DEBUG OUTPUT :: inserting [" << service << "]" << std::endl;
    assert(service != __null);
    services[index] = service;
  }

protected:
  ServiceMap_t services;

  std::shared_ptr<ServiceInterfaceBase> get_service_interface(std::type_index);
};
