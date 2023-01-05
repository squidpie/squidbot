#pragma once

#include <typeindex>

#include "logging.h"
#include "service_loader.h"
#include "servicelib.h"

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
  ~ServiceManager() {}

  void load(std::shared_ptr<ServiceContext> context);
  void register_service(const std::type_index, std::shared_ptr<ServiceBase>);

  /*
  Caller must check for NULL pointer on return
   */
  template <class T>
  std::shared_ptr<typename T::plugin_interface_t> get_interface() {
    if (!is_service_available(std::type_index(typeid(T)))) {
      PLOGE << "No Interface found for type <" << typeid(T).name() << ">";
      return nullptr;
    }
    return std::dynamic_pointer_cast<typename T::plugin_interface_t>(
        get_service_interface(std::type_index(typeid(T))));
  };


protected:
  ServiceMap_t services;

  std::shared_ptr<ServiceInterfaceBase> get_service_interface(std::type_index);
  bool is_service_available(std::type_index);
};
