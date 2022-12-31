#pragma once

#include <cstdint>
#include <string>
#include <dlfcn.h>
#include <iostream>

#include "manager.h"
#include "service.h"
#include "utils.h"

class ServiceContainerBase {};

class ServiceManager: Manager {
public:
  ServiceManager() {}
  ServiceManager(std::string lpath) { lib_dir = lpath; }
  ~ServiceManager() {}
  void init();
  void load(ServiceContainerBase* service);
  void unload(ServiceContainerBase* service);

  protected:
    std::string lib_dir = LIB_DIR;
};
