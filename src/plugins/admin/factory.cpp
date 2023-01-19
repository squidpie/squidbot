#include "admin.h"

// Factory
extern "C" void create(std::string __lib_path,
                       std::shared_ptr<Context> context) {
  static bool is_log_init = false;
  auto plugin_context = std::dynamic_pointer_cast<CoreContext>(context);

  if (!is_log_init) {
    //plog::init(plog::debug, plugin_context->plog_appender);
    is_log_init = true;
  }
  
  PLOGD << "Admin Plugin Create";
  auto plugin = std::make_shared<Plugin<Admin>>(plugin_context);
  plugin_context->plugin_manager->register_plugin<Admin>(__lib_path, plugin);
  plugin->start();
}