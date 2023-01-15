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

void PluginManager::_unload(std::type_index index) {
  plugins->erase(index);
}

void PluginManager::_reload(std::type_index index) {
  auto lib_path = plugins->at(index).first;
  _unload(index);
  lib_loader->load_lib(lib_path);
}