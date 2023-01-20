/*
factory.cpp
Copyright (C) 2023  Squidpie
 */

#include "admin.h"

// Factory
extern "C" void create(std::string lib_path, std::shared_ptr<Context> context) {
  PLOGD << "Admin Plugin Create";
  auto plugin_context{std::dynamic_pointer_cast<CoreContext>(context)};
  auto plugin{std::make_shared<Plugin<Admin>>(plugin_context)};
  plugin_context->plugin_manager->register_plugin<Admin>(lib_path, plugin);
  plugin->start();
}