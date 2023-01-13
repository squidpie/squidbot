#pragma once
#include <memory>

#include "utils/context.h"
#include "logging.h"

class EventServerBase;
class ServiceManagerBase;

class CoreContext : virtual public Context {
public:
  CoreContext(plog::IAppender *plog_appender,
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