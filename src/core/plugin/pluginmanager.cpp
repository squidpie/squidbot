#include "pluginmanager.h"

void PluginManager::load(std::shared_ptr<CoreContext> context) {
  clear_unload_threads();
  lib_loader = std::make_unique<LibLoader<PluginLoader>>(context);
}

void PluginManager::_register_plugin(std::type_index index, std::pair<std::string, std::shared_ptr<PluginBase>> entry) {
  plugins->insert({index, entry});
}

void PluginManager::unload()  {
  clear_unload_threads();
  lib_loader.reset();
  plugins->clear();
}

void PluginManager::_unload(std::type_index index) {
  clear_unload_threads();
  unload_threads->push_back(std::make_unique<std::thread>(&PluginManager::unload_actions, this, index));
}

void PluginManager::_reload(std::type_index index) {
  clear_unload_threads();
  unload_threads->push_back(std::make_unique<std::thread>(&PluginManager::reload_actions, this, index));
}

void PluginManager::load_plugin(std::string lib_name) {
  clear_unload_threads();
  lib_loader->load_lib(lib_name);
}

void PluginManager::unload_actions(std::type_index index) {
  plugins->at(index).second->stop();
  plugins->erase(index);
}

void PluginManager::reload_actions(std::type_index index) {
  auto lib_path = plugins->at(index).first;
  unload_actions(index);
  lib_loader->load_lib(lib_path);
}

void PluginManager::clear_unload_threads() {
  while(unload_threads->size() != 0) {
    auto thread = std::move(unload_threads->back());
    if (thread) thread->join();
    unload_threads->pop_back();
  }
}