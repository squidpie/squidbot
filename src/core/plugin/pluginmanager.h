#pragma once

#include <memory>
#include <typeindex>

#include "plugin.h"
#include "pluginloader.h"
#include "lib/core_context.h"

typedef std::unordered_map<std::type_index, std::pair<std::string, std::shared_ptr<PluginBase>>> PluginMap_t;

class PluginManagerBase {
  public:
    PluginManagerBase() {}
    virtual ~PluginManagerBase() {}
    virtual void load(std::shared_ptr<CoreContext>) = 0;
    virtual void unload() = 0;
    template<class P> void register_plugin(std::string lib_path, std::shared_ptr<PluginBase> plugin) {
      _register_plugin(std::type_index(typeid(P)), std::make_pair(lib_path, plugin));
    }
    template<class P> void unload_plugin() {
      _unload(std::type_index(typeid(P)));
    }

    template<class P> void reload_plugin() {
      _reload(std::type_index(typeid(P)));
    }
    
    protected:
      virtual void _register_plugin(std::type_index, std::pair<std::string, std::shared_ptr<PluginBase>>) = 0;
      virtual void _unload(std::type_index) = 0;
      virtual void _reload(std::type_index) = 0;
};

class PluginManager : virtual public PluginManagerBase {
  public:
    PluginManager() {}
    ~PluginManager() {}
    void load(std::shared_ptr<CoreContext>) override;
    void unload() override;

    #ifdef _GTEST_COMPILE
    auto inject(auto _plugins) { plugins = _plugins; }
    #endif

    protected:
      std::unique_ptr<LibLoader<PluginLoader>> lib_loader;
      std::shared_ptr<PluginMap_t> plugins = std::make_shared<PluginMap_t>();

      void _register_plugin(std::type_index, std::pair<std::string, std::shared_ptr<PluginBase>>) override;
      void _unload(std::type_index) override;
      void _reload(std::type_index) override;
};