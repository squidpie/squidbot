#include "mockplugin.h"

// Factory
extern "C" void create(std::string lib_path,
                       std::shared_ptr<Context> context) {
  static bool is_log_init = false;
  auto plugin_context = std::dynamic_pointer_cast<CoreContext>(context);

  if (!is_log_init) {
    std::cerr << "WTF" << std::endl;
    //plog::init(plog::debug, plugin_context->plog_appender);
    is_log_init = true;
  }

  PLOGD << "MockPlugin Create";
  auto plugin = std::make_shared<Plugin<MockPlugin>>(plugin_context);
  plugin_context->plugin_manager->register_plugin<MockPlugin>(lib_path, plugin);
  plugin->start();
}