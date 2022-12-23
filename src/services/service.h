#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <dlfcn.h>
#include <iostream>
#include <system_error>

#include "logging.h"

class ServiceRuntimeError : public std::runtime_error {
public:
  ServiceRuntimeError() : std::runtime_error("ServiceRuntimeError") {}
};
class ServiceLibError : public std::system_error {};

class ServiceBase {
public:
  virtual ~ServiceBase() {}
  virtual bool run() = 0;

protected:
  virtual void load() = 0;
  virtual void unload() = 0;
private:
};

// Must be defined after ServiceBase and before ServiceContainer
typedef ServiceBase* create_t();
typedef void destroy_t(ServiceBase*);

typedef class ServiceContainer {
public:
    ServiceContainer(std::string name) {
      ServiceContainer::name = name;
    }
    bool run();
    void gen_lib_path(std::string dir);
    void init();
    void create();
    void destroy();

protected:
  void need_service();
  std::string name;
  std::string lib_path;
  std::uint_fast64_t guid;
  ServiceBase* service;
  create_t* create_service;
  destroy_t* destroy_service;
} ServiceContainer_t;