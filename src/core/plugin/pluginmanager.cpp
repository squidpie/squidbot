#include "pluginmanager.h"

void PluginManager::load(std::shared_ptr<CoreContext> context) {
  lib_loader = std::make_unique<LibLoader<PluginLoader>>(context);
}

void PluginManager::_register_plugin(std::type_index index, std::pair<std::string, std::shared_ptr<PluginBase>> entry) {
  plugins->insert({index, entry});
}

void PluginManager::unload()  {
  lib_loader.reset();
  plugins->clear();
}