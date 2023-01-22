/*
templatename.h
Copyright (C) 2023 Squidpie
 */

#pragma once

#include <memory>

#include "templatename/events.h"
#include "templatename/version.h"
#include "core/corelib.h"

class TemplateNameData : virtual public ServiceDataBase {
 public:
  ~TemplateNameData() {}
};

class TemplateNameRunActionContext : virtual public RunActionContextBase {
 public:
  TemplateNameRunActionContext(std::shared_ptr<EventClientBase> event_client,
                              std::shared_ptr<TemplateNameData> data)
      : event_client(event_client), data(data) {}
  ~TemplateNameRunActionContext() {}
  std::shared_ptr<EventClientBase> event_client;
  std::shared_ptr<TemplateNameData> data;
};

class TemplateNameRunAction : virtual public RunActionBase {
 public:
  typedef TemplateNameRunActionContext context_t;
  explicit TemplateNameRunAction(
      std::shared_ptr<TemplateNameRunActionContext> context)
      : event_client(context->event_client), data(context->data) {}
  ~TemplateNameRunAction() {}
  void run_action();

 protected:
  std::shared_ptr<EventClientBase> event_client;
  std::shared_ptr<TemplateNameData> data;
};

class TemplateNamePluginInterface : virtual public ServiceInterfaceBase {
 public:
  explicit TemplateNamePluginInterface(std::shared_ptr<TemplateNameData> data)
      : data(data) {}
  ~TemplateNamePluginInterface() {}

 protected:
  std::shared_ptr<TemplateNameData> data;
};

class TemplateNameExternalInterface : virtual public ServiceInterfaceBase {
 public:
  ~TemplateNameExternalInterface() {}
};

class TemplateName {
 public:
  typedef TemplateNameRunAction run_action_t;
  typedef TemplateNameData data_t;
  typedef TemplateNamePluginInterface plugin_interface_t;
  typedef TemplateNameExternalInterface external_interface_t;
  static const uint service_version{TEMPLATENAME_VERSION};
  static const uint core_version{CORE_VERSION};
};
