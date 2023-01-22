/*
adminservice.h
Copyright (C) 2023 Squidpie
 */
 
#pragma once

#include "adminservice/events.h"
#include "adminservice/version.h"
#include "corelib.h"

class AdminServiceData : virtual public ServiceDataBase {
  public:
    ~AdminServiceData() {}
};

class AdminServiceRunActionContext : virtual public RunActionContextBase {
public:
  AdminServiceRunActionContext(std::shared_ptr<EventClientBase> event_client,
                               std::shared_ptr<AdminServiceData> data)
    : event_client(event_client), data(data) {}
  ~AdminServiceRunActionContext() {}
  std::shared_ptr<EventClientBase> event_client;
  std::shared_ptr<AdminServiceData> data;
};

class AdminServiceRunAction : virtual public RunActionBase {
  public:
  typedef AdminServiceRunActionContext context_t;
  AdminServiceRunAction(std::shared_ptr<AdminServiceRunActionContext> context) : event_client(context->event_client), data(context->data) {}
  ~AdminServiceRunAction() {}
  void run_action();

  protected:
    std::shared_ptr<EventClientBase> event_client;
    std::shared_ptr<AdminServiceData> data;
};

class AdminServicePluginInterface : virtual public ServiceInterfaceBase {
  public:
    AdminServicePluginInterface(std::shared_ptr<AdminServiceData> data) : data(data) {}
    ~AdminServicePluginInterface() {}
  protected:
    std::shared_ptr<AdminServiceData> data;
};

class AdminServiceExternalInterface : virtual public ServiceInterfaceBase {
  public:
    ~AdminServiceExternalInterface() {}
};

class AdminService {
  public:
  typedef AdminServiceRunAction run_action_t;
  typedef AdminServiceData data_t;
  typedef AdminServicePluginInterface plugin_interface_t;
  typedef AdminServiceExternalInterface external_interface_t;
  static const uint service_version{ADMINSERVICE_VERSION};
  static const uint core_version{CORE_VERSION};
};