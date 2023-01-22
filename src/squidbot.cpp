/*
        SQUIDBOT
        STREAM AUTOMATION FRAMEWORK
        Copyright (C) 2023  Squidpie
*/

#include <csignal>

#include "core/corelib.h"
#include "logging/logging.h"
#include "utils/defines.h"

#include "admin/admin.h"

static bool is_main_running{true};

int main(int argc, char **argv) {
  auto squidbot_lib_dir = std::getenv("SQUIDBOT_LIB_DIR");
  if (!squidbot_lib_dir) {
    std::cerr << "Unable to load ${SQUIDBOT_LIB_DIR}" << std::endl;
    exit(1);
  }

  if (argc > 1) {
    if (std::string(argv[1]) == "--test") {
      std::cout << "Executing Test Mode" << std::endl;
      is_main_running = false;
    }
  }

  plog::init(plog::debug, "squidbot.log");
  plog_shared_init(plog::debug, plog::get(), squidbot_lib_dir);
  PLOGI << "Logging Initialized";

  // capture ctrl-c to exit program
  signal(SIGINT, [](int signum) {
    PLOGI << "Exit on sigint " << signum;
    is_main_running = false;
  });

  auto event_server = std::make_shared<EventServer>();
  auto plugin_manager = std::make_shared<PluginManager>();
  auto service_manager = std::make_shared<ServiceManager>();

  std::shared_ptr<CoreContext> context =
      std::make_shared<CoreContext>(plog::get(), event_server, service_manager,
                                    plugin_manager, squidbot_lib_dir);

  PLOGI << "Starting Event Server";
  event_server->start();

  PLOGI << "Loading Services";
  service_manager->load(context);
  PLOGI << "Loading Plugins";
  plugin_manager->load(context);

  // reload_plugin is not intended to be called in such a manner
  // testing purposes only
  PLOGD << "Reloading Admin Plugin";
  plugin_manager->reload_plugin<Admin>();

  // Main Loop
  std::cout << "Running..." << std::endl;
  while (is_main_running) {
    pause();
  }
  std::cout << std::endl;  // courtesy newline

  PLOGI << "Stopping Event Server";
  event_server->stop();

  PLOGI << "Unloading Plugins";
  plugin_manager->unload();
  PLOGI << "Unloading Services";
  service_manager->unload();
}
