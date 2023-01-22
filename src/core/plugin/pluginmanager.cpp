/*
pluginmanager.cpp
Copyright (C) 2023  Squidpie
 */

#include <memory>
#include <string>
#include <utility>

#include "plugin/plugin.h"
#include "plugin/pluginmanager.h"

void PluginManager::load(std::shared_ptr<CoreContext> context) {
  PLOGD << "Loading";
  clear_unload_threads();
  lib_loader = std::make_unique<LibLoader<PluginLoader>>(context);
}

void PluginManager::_register_plugin(
    std::type_index index,
    std::pair<std::string, std::shared_ptr<PluginBase>> entry) {
  if (entry.second->core_version() != CORE_VERSION) {
    LOGE << "::CORE_VERSION MISMATCH::";
    return;
  }
  plugins->insert({index, entry});
}

void PluginManager::unload() {
  PLOGD << "Unloading";
  clear_unload_threads();
  plugins->clear();
  lib_loader.reset();
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
  PLOGD << "Loading plugin " << lib_name;
  clear_unload_threads();
  lib_loader->load_lib(lib_name);
}

void PluginManager::unload_actions(std::type_index index) {
  PLOGD << "Executing Unload Actions";
  const std::lock_guard<std::mutex> pguard(plock);
  plugins->at(index).second->stop();
  plugins->erase(index);
}

void PluginManager::reload_actions(std::type_index index) {
  PLOGD << "Executing Reload Actions";
  auto lib_path = plugins->at(index).first;
  unload_actions(index);
  lib_loader->load_lib(lib_path);
}

void PluginManager::clear_unload_threads() {
  PLOGD << "Clearing Unload Threads";
  while (unload_threads->size() != 0) {
    auto thread = std::move(unload_threads->back());
    if (thread) {
      thread->join();
    }
    unload_threads->pop_back();
  }
}

void PluginManager::service_unload_notify(std::type_index index) {
  PLOGD << "service_unload_notify";
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
  PLOGD << "received warn event";
  for (const auto &[key, entry] : *plugins) {
    auto plugin = entry.second;
    if (plugin->is_dependent(*index)) {
      plugin->stop();
    }
  }
  return std::thread(&PluginManager::service_reload_notify, this, plugin_mutex,
                     index);
}

void PluginManager::service_reload_notify(
    std::shared_ptr<std::mutex> plugin_mutex,
    std::shared_ptr<std::type_index> index) {

  PLOGD << "notifying plugins";
  plugin_mutex->lock();
  const std::lock_guard<std::mutex> pguard(plock);
  for (const auto &[key, entry] : *plugins) {
    auto plugin = entry.second;
    if (plugin->is_dependent(*index)) {
      plugin->reload();
    }
  }
  plugin_mutex->unlock();
}
