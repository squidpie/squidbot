#pragma once

#include "core.h"
#include "service_loader.h"
#include "utils/lib_loader.h"

class ServiceManagerBase {
public:
  virtual ~ServiceManagerBase() {}
  virtual void load(std::shared_ptr<CoreContext>) = 0;
  virtual void unload() = 0;

  template <class S>
  void register_service(std::string lib_path,
                        std::shared_ptr<ServiceBase> service) {
    _register_service(std::type_index(typeid(S)),
                      std::make_pair(lib_path, service));
  }

  /*
  Caller must check for nullptr
   */
  template <class T>
  std::shared_ptr<typename T::plugin_interface_t> get_interface(const uint plugin_version) {
    if (plugin_version != T::service_version) {
      // LOG ERROR
      return nullptr;
    }
    return std::dynamic_pointer_cast<typename T::plugin_interface_t>(
        _get_interface(std::type_index(typeid(T))));
  }

  template <class S> void unload_service() {
    _unload(std::type_index(typeid(S)));
  }
  virtual void load_service(std::string) = 0;

  template <class S> void reload_service() {
    _reload(std::make_shared<std::type_index>(typeid(S)));
  }

protected:
  virtual void _register_service(
      std::type_index,
      std::pair<std::string, std::shared_ptr<ServiceBase>>) = 0;
  virtual std::shared_ptr<ServiceInterfaceBase>
  _get_interface(std::type_index index) = 0;
  virtual void _unload(std::type_index) = 0;
  virtual void _reload(std::shared_ptr<std::type_index>) = 0;
};

class ServiceManager : virtual public ServiceManagerBase,
                       public std::enable_shared_from_this<ServiceManager> {
public:
  ServiceManager() {}
  ~ServiceManager() {}

  void load(std::shared_ptr<CoreContext>) override;
  void unload() override;
  void load_service(std::string) override;
  void _register_service(
      const std::type_index,
      std::pair<std::string, std::shared_ptr<ServiceBase>>) override;

  std::shared_ptr<ServiceInterfaceBase>
  _get_interface(std::type_index index) override;

#ifdef _GTEST_COMPILE
  void inject(auto _services) { services = _services; }
#endif

protected:
  std::shared_ptr<CoreContext> context;
  std::shared_ptr<ServiceMap_t> services = std::make_shared<ServiceMap_t>();
  std::unique_ptr<LibLoader<ServiceLoader>> libloader;

  bool is_service_available(std::type_index);
  void _unload(std::type_index) override;
  void _reload(std::shared_ptr<std::type_index>) override;
};
