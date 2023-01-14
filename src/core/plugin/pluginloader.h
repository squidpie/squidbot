#pragma once

#include "utils/lib_loader.h"

typedef void (*create_plugin_t)(std::string, std::shared_ptr<Context>);

class PluginLoader {
public:
  static constexpr std::string_view lib = "/plugins";
  typedef create_plugin_t create_t;
  ~PluginLoader() {}
};