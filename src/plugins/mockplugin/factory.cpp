#include "mockplugin.h"

// Factory
extern "C" void create(std::string __lib_path,
                       std::shared_ptr<Context> context) {
  auto plugin_context = std::dynamic_pointer_cast<CoreContext>(context);
  auto plugin = std::make_shared<Plugin<MockPlugin>>(plugin_context);
  plugin->start();
}