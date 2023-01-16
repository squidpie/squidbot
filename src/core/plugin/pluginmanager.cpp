#include "pluginmanager.h"
#include <condition_variable>

void PluginManager::load(std::shared_ptr<CoreContext> context) {
  clear_unload_threads();
  lib_loader = std::make_unique<LibLoader<PluginLoader>>(context);
}

void PluginManager::_register_plugin(
    std::type_index index,
    std::pair<std::string, std::shared_ptr<PluginBase>> entry) {
  plugins->insert({index, entry});
}

void PluginManager::unload() {
  clear_unload_threads();
  lib_loader.reset();
  plugins->clear();
}

void PluginManager::_unload(std::type_index index) {
  clear_unload_threads();
  unload_threads->push_back(std::make_unique<std::thread>(
      &PluginManager::unload_actions, this, index));
}

void PluginManager::_reload(std::type_index index) {
  clear_unload_threads();
  unload_threads->push_back(std::make_unique<std::thread>(
      &PluginManager::reload_actions, this, index));
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
  while (unload_threads->size() != 0) {
    auto thread = std::move(unload_threads->back());
    if (thread)
      thread->join();
    unload_threads->pop_back();
  }
}

void PluginManager::service_unload_notify(std::type_index index) {
  for (auto entry = plugins->begin(); entry != plugins->end();) {
    auto plugin = entry->second.second;
    if (plugin->is_dependent(index)) {
      plugins->erase(entry++);
    } else {
      ++entry;
    }
  }
}

std::thread
PluginManager::service_reload_warn(std::shared_ptr<std::mutex> plugin_mutex,
                                   std::shared_ptr<std::type_index> index) {
  for (const auto &[key, entry] : *plugins) {
    auto plugin = entry.second;
    if (plugin->is_dependent(*index)) {
      plugin->stop();
    }
  }
  return std::thread(&PluginManager::service_reload_notify, this,
                            plugin_mutex, index);
}

void PluginManager::service_reload_notify(
    std::shared_ptr<std::mutex> plugin_mutex, std::shared_ptr<std::type_index> index) {
  plugin_mutex->lock();
  for (const auto &[key, entry] : *plugins) {
    auto plugin = entry.second;
    if (plugin->is_dependent(*index)) {
      plugin->reload();
    }
  }
  plugin_mutex->unlock();
}