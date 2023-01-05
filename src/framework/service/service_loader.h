#pragma once

#include <vector>
#include <string>

#include "servicelib.h"

class ServiceLoader {
  public:
   typedef create_service_t create_t;
  ~ServiceLoader() {}
};

/*
class PluginLoader {
  public:
    typedef PluginMap_t data_t;
    ~PluginLoader() {}
    PluginMap_t load_all(std::string);
}
*/
