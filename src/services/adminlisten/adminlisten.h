/*
adminlisten.h
Copyright (C) 2023 Squidpie
 */

#pragma once

#include "adminlisten/events.h"
#include "adminlisten/version.h"
#include "corelib.h"

class AdminListenData : virtual public ServiceDataBase {
  public:
    ~AdminListenData() {}
};

class AdminListenRunActionContext : virtual public RunActionContextBase {
public:
  AdminListenRunActionContext(std::shared_ptr<EventClientBase> event_client,
                               std::shared_ptr<AdminListenData> data)
    : event_client(event_client), data(data) {}
  ~AdminListenRunActionContext() {}
  std::shared_ptr<EventClientBase> event_client;
  std::shared_ptr<AdminListenData> data;
};

class AdminListenRunAction : virtual public RunActionBase {
  public:
  typedef AdminListenRunActionContext context_t;
  AdminListenRunAction(std::shared_ptr<AdminListenRunActionContext> context) : event_client(context->event_client), data(context->data) {}
  ~AdminListenRunAction() {}
  void run_action();

  protected:
    std::shared_ptr<EventClientBase> event_client;
    std::shared_ptr<AdminListenData> data;
};

class AdminListenPluginInterface : virtual public ServiceInterfaceBase {
  public:
    AdminListenPluginInterface(std::shared_ptr<AdminListenData> data) : data(data) {}
    ~AdminListenPluginInterface() {}
  protected:
    std::shared_ptr<AdminListenData> data;
};

class AdminListenExternalInterface : virtual public ServiceInterfaceBase {
  public:
    ~AdminListenExternalInterface() {}
};

class AdminListen {
  public:
  typedef AdminListenRunAction run_action_t;
  typedef AdminListenData data_t;
  typedef AdminListenPluginInterface plugin_interface_t;
  typedef AdminListenExternalInterface external_interface_t;
  static const uint service_version{ADMINLISTEN_VERSION};
  static const uint core_version{CORE_VERSION};
};