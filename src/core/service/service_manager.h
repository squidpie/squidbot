#pragma once

#include <typeindex>

#include "logging.h"
#include "service.h"
#include "service_loader.h"
#include "servicelib.h"

class ServiceManagerBase {
public:
  virtual ~ServiceManagerBase() {}
  virtual void register_service(std::type_index,
                                std::shared_ptr<ServiceBase>) = 0;
  template <class T> std::shared_ptr<typename T::plugin_interface_t> get_interface() {
    return std::dynamic_pointer_cast<typename T::plugin_interface_t>(_get_interface(std::type_index(typeid(T))));
  }
  protected:
    virtual std::shared_ptr<ServiceInterfaceBase> _get_interface(std::type_index index) = 0;
};

class ServiceManager : virtual public ServiceManagerBase,
                       public std::enable_shared_from_this<ServiceManager> {
public:
  ServiceManager() {}
  ~ServiceManager() {}

  void load(std::shared_ptr<CoreContext> context);
  void register_service(const std::type_index, std::shared_ptr<ServiceBase>) override;

  /*
  Caller must check for NULL pointer on return

  template <class T>
  std::shared_ptr<typename T::plugin_interface_t> get_interface() {
  */
  std::shared_ptr<ServiceInterfaceBase> _get_interface(std::type_index index) override {
    if (!is_service_available(index)) {
      PLOGE << "No Interface found";
      return nullptr;
    }
    return get_service_interface(index);
  };


protected:
  ServiceMap_t services;

  std::shared_ptr<ServiceInterfaceBase> get_service_interface(std::type_index);
  bool is_service_available(std::type_index);
};
