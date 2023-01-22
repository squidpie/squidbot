/*
admin.h
Copyright (C) 2023  Squidpie
 */

#pragma once

#include "core/corelib.h"
#include "adminlisten/adminlisten.h"

class AdminAction : virtual public PluginActionBase {
 public:
  AdminAction(InterfaceMap_t *interfaces, std::shared_ptr<PluginDataBase>);
  ~AdminAction() {}
  void trigger_action(Event) override;

 protected:
  InterfaceMap_t *interfaces;
  std::shared_ptr<AdminListenPluginInterface> adminlisten;
};

class AdminFilter : virtual public PluginFilterBase {
 public:
  ~AdminFilter() {}
  bool is_trigger(Event) override;
};

class Admin {
 public:
  typedef AdminAction action_t;
  typedef AdminFilter filter_t;
  explicit Admin(std::shared_ptr<EventClientBase>) {}
  ~Admin() {}
  InterfaceMap_t create_interfaces(std::shared_ptr<CoreContext>);
  static const uint core_version{CORE_VERSION};
};