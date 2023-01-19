#include <iostream>
#include <stdio.h>
#include <thread>
#include <vector>

#include "corelib.h"
#include "logging.h"
#include "utils/defines.h"

#include "mockservice/mockservice.h"
#include "admin/admin.h"

int main(int argc, char **argv) {
  plog::init(plog::debug, "squidbot.log");
  auto event_server = std::make_shared<EventServer>();
  auto plugin_manager = std::make_shared<PluginManager>();
  auto service_manager = std::make_shared<ServiceManager>();

  std::shared_ptr<CoreContext> context = std::make_shared<CoreContext>(
      plog::get(), event_server, service_manager, plugin_manager, LIB_DIR);
  service_manager->load(context);
  plugin_manager->load(context);
  plugin_manager->reload_plugin<Admin>();
  plugin_manager->unload();
  service_manager->unload();
}