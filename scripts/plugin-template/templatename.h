/*
templatename.h
Copyright (C) 2023  Squidpie
 */

#pragma once

#include "core/corelib.h"

// include the necessary service headers
// #include "servicename/servicename.h"

class TemplateNameAction : virtual public PluginActionBase {
 public:
  TemplateNameAction(InterfaceMap_t *interfaces, std::shared_ptr<PluginDataBase>);
  ~TemplateNameAction() {}
  void trigger_action(Event) override;

 protected:
  InterfaceMap_t *interfaces;
  // declare your interfaces for the action
  // std::shared_ptr<ServiceNamePluginInterface> servicename;
};

class TemplateNameFilter : virtual public PluginFilterBase {
 public:
  ~TemplateNameFilter() {}
  bool is_trigger(Event) override;
};

class TemplateName {
 public:
  typedef TemplateNameAction action_t;
  typedef TemplateNameFilter filter_t;
  explicit TemplateName(std::shared_ptr<EventClientBase>) {}
  ~TemplateName() {}
  InterfaceMap_t create_interfaces(std::shared_ptr<CoreContext>);
  static const uint core_version{CORE_VERSION};
};