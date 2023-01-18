#pragma once

#include <mutex>
#include <thread>

#include "core.h"
#include "pluginloader.h"

typedef std::unordered_map<std::type_index,
                           std::pair<std::string, std::shared_ptr<PluginBase>>>
    PluginMap_t;

class PluginManagerBase {
public:
  PluginManagerBase() {}
  virtual ~PluginManagerBase() {}
  virtual void load(std::shared_ptr<CoreContext>) = 0;
  virtual void unload() = 0;
  virtual void load_plugin(std::string) = 0;
  virtual void service_unload_notify(std::type_index) = 0;
  virtual std::thread service_reload_warn(std::shared_ptr<std::mutex>,
                                          std::shared_ptr<std::type_index>) = 0;
  template <class P>
  void register_plugin(std::string lib_path,
                       std::shared_ptr<PluginBase> plugin) {
    _register_plugin(std::type_index(typeid(P)),
                     std::make_pair(lib_path, plugin));
  }
  template <class P> void unload_plugin() {
    _unload(std::type_index(typeid(P)));
  }

  template <class P> void reload_plugin() {
    _reload(std::type_index(typeid(P)));
  }

protected:
  virtual void
      _register_plugin(std::type_index,
                       std::pair<std::string, std::shared_ptr<PluginBase>>) = 0;
  virtual void _unload(std::type_index) = 0;
  virtual void _reload(std::type_index) = 0;
};

class PluginManager : virtual public PluginManagerBase {
public:
  PluginManager() {}
  ~PluginManager() { clear_unload_threads(); }
  void load(std::shared_ptr<CoreContext>) override;
  void unload() override;
  void load_plugin(std::string) override;
  void service_unload_notify(std::type_index) override;
  std::thread service_reload_warn(std::shared_ptr<std::mutex>,
                                  std::shared_ptr<std::type_index>) override;

#ifdef _GTEST_COMPILE
  auto inject(auto _plugins, auto _unload_threads) {
    plugins = _plugins;
    unload_threads = _unload_threads;
  }
#endif

protected:
  std::mutex plock;
  std::unique_ptr<LibLoader<PluginLoader>> lib_loader;
  std::shared_ptr<PluginMap_t> plugins = std::make_shared<PluginMap_t>();
  std::shared_ptr<std::vector<std::unique_ptr<std::thread>>> unload_threads =
      std::make_shared<std::vector<std::unique_ptr<std::thread>>>();

  void _register_plugin(
      std::type_index,
      std::pair<std::string, std::shared_ptr<PluginBase>>) override;
  void _unload(std::type_index) override;
  void _reload(std::type_index) override;
  void unload_actions(std::type_index);
  void reload_actions(std::type_index);
  void clear_unload_threads();
  void service_reload_notify(std::shared_ptr<std::mutex>,
                             std::shared_ptr<std::type_index>);
};