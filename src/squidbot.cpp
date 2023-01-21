/*
        SQUIDBOT
        STREAM AUTOMATION FRAMEWORK
        Copyright (C) 2023  Squidpie
*/

#include <signal.h>

#include "corelib.h"
#include "logging.h"
#include "utils/defines.h"

#include "admin/admin.h"
#include "mockservice/mockservice.h"

static bool is_main_running{true};

int main(int argc, char **argv) {
  plog::init(plog::debug, "squidbot.log");
  plog_shared_init(plog::debug, plog::get(), LIB_DIR);
  PLOGD << "Logging Initialized";

  // capture ctrl-c to exit program
  signal(SIGINT, [](int signum){
    PLOGD << "Exit on sigint " << signum;
    is_main_running = false;
  });

  auto squidbot_lib_dir = std::getenv("SQUIDBOT_LIB_DIR");
  if (!squidbot_lib_dir) {
    std::cerr << "Unable to load ${SQUIDBOT_LIB_DIR}" << std::endl;
    exit(1);
  }

  auto event_server = std::make_shared<EventServer>();
  auto plugin_manager = std::make_shared<PluginManager>();
  auto service_manager = std::make_shared<ServiceManager>();

  // Load
  std::shared_ptr<CoreContext> context = std::make_shared<CoreContext>(
      plog::get(), event_server, service_manager, plugin_manager, squidbot_lib_dir);

  PLOGD << "Starting Event Server";
  event_server->start();

  PLOGD << "Loading context";
  service_manager->load(context);
  plugin_manager->load(context);

  // reload_plugin is not intended to be called in such a manner
  // testing purposes only
  PLOGD << "Reloading Admin Plugin";
  plugin_manager->reload_plugin<Admin>();

  // Main Loop
  while(is_main_running){ sleep(1); }
  std::cout << std::endl; // courtsey newline

  // Unload
  PLOGD << "Stopping Event Server";
  event_server->stop();

  PLOGD << "Unloading Context";
  plugin_manager->unload();
  service_manager->unload();
}