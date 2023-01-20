/*
core_context.h
Copyright (C) 2023  Squidpie
 */

#pragma once
#include <memory>

#include "utils/context.h"
#include "logging.h"
#include "core.h"

class CoreContext : virtual public Context {
public:
  CoreContext(plog::IAppender *plog_appender,
              std::shared_ptr<EventServerBase> event_server,
              std::shared_ptr<ServiceManagerBase> service_manager,
              std::shared_ptr<PluginManagerBase> plugin_manager,
              std::string lib_dir)
      : Context(lib_dir), plog_appender(plog_appender),
        event_server(event_server), service_manager(service_manager), plugin_manager(plugin_manager) {}
  virtual ~CoreContext() {}
  plog::IAppender *plog_appender;
  std::shared_ptr<EventServerBase> event_server;
  std::shared_ptr<ServiceManagerBase> service_manager;
  std::shared_ptr<PluginManagerBase> plugin_manager;
  using Context::lib_dir;
};