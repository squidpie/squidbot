#pragma once

#include <cstdint>
#include <dlfcn.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <system_error>

#include "events.h"
#include "logging.h"
#include "utils.h"

class ServiceRuntimeError : public std::runtime_error {
public:
  ServiceRuntimeError() : std::runtime_error("ServiceRuntimeError") {}
};
class ServiceLibError : public std::system_error {};

class ServiceBase {
public:
  ServiceBase(Context_t *context) {
    plog::init(plog::debug, context->plog_appender);
    buffer_client = context->buffer_server->create_client();
    PLOGD << "ServiceBase client = " << buffer_client;
    PLOGD << "ServiceBase server = " << context->buffer_server;
  }
  virtual ~ServiceBase() {}
  virtual void run() = 0;

protected:
  virtual void load() = 0;
  virtual void unload() = 0;

  bool is_running;
  EventClientBase* buffer_client;

private:
};

// Must be defined after ServiceBase and before ServiceContainer
typedef ServiceBase *create_t(Context_t *);
typedef void destroy_t(ServiceBase *);

typedef class ServiceContainer {
public:
  Context_t *context;
  ServiceContainer(std::string _name, Context_t *_context) {
    name = _name;
    context = _context;
  }
  void run();
  void gen_lib_path(std::string dir);
  void init();
  void create();
  void destroy();

protected:
  void need_service();
  std::string name;
  std::string lib_path;
  std::uint_fast64_t guid;
  ServiceBase *service;
  create_t *create_service;
  destroy_t *destroy_service;
} ServiceContainer_t;