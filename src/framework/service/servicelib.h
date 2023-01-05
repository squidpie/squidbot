#pragma once
/*
            Forward Declarations
    :: WARNING Do Not Change the Ordering ::

 */

#include <memory>
#include <stdexcept>
#include <system_error>
#include <typeindex>
#include <unordered_map>

#include "event_server.h"
#include "logging.h"
#include "utils.h"

class ServiceBase;
class ServiceInterfaceBase;

template <class S> class Service;

class ServiceManagerBase;
class ServiceLoader;
class ServiceManager;

class ServiceContext : virtual public Context {
public:
  ServiceContext(plog::IAppender *plog_appender,
                 std::shared_ptr<EventServerBase> event_server,
                 std::shared_ptr<ServiceManagerBase> service_manager,
                 std::string lib_dir)
      : Context(lib_dir), plog_appender(plog_appender),
        event_server(event_server), service_manager(service_manager) {}

  plog::IAppender *plog_appender;
  std::shared_ptr<EventServerBase> event_server;
  std::shared_ptr<ServiceManagerBase> service_manager;
  using Context::lib_dir;
};

enum ServiceStatus { SERVICE_STATUS_NULL, SERVICE_STATUS_OK };

typedef void (*create_service_t)(std::shared_ptr<Context>);
typedef void (*destroy_service_t)(std::unique_ptr<ServiceBase>);

class ServiceRuntimeError : public std::runtime_error {
public:
  ServiceRuntimeError() : std::runtime_error("ServiceRuntimeError") {}
};
class ServiceLibError : public std::system_error {};

typedef std::unordered_map<std::type_index,
                           std::shared_ptr<ServiceBase>>
    ServiceMap_t;