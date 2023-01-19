/*
        SQUIDBOT
        STREAM AUTOMATION FRAMEWORK
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

  signal(SIGINT, [](int signum){
    PLOGD << "Exit on sigint " << signum;
    is_main_running = false;
  });

  auto event_server = std::make_shared<EventServer>();
  auto plugin_manager = std::make_shared<PluginManager>();
  auto service_manager = std::make_shared<ServiceManager>();

  std::shared_ptr<CoreContext> context = std::make_shared<CoreContext>(
      plog::get(), event_server, service_manager, plugin_manager, LIB_DIR);

  PLOGD << "Starting Event Server";
  event_server->start();

  PLOGD << "Loading context";
  service_manager->load(context);
  plugin_manager->load(context);

  // reload_plugin is not intended to be called in such a manner
  // testing purposes only
  PLOGD << "Reloading Admin Plugin";
  plugin_manager->reload_plugin<Admin>();

  while(is_main_running){}

  PLOGD << "Stopping Event Server";
  event_server->stop();

  PLOGD << "Unloading Context";
  plugin_manager->unload();
  service_manager->unload();
}