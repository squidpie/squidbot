#pragma once

#include "corelib.h"
#include "mockservice/mockservice.h"

class AdminAction : virtual public PluginActionBase {
public:
  AdminAction(InterfaceMap_t *interfaces, std::shared_ptr<PluginDataBase>);
  ~AdminAction() {}
  void trigger_action(Event) override;

protected:
  InterfaceMap_t *interfaces;
  std::shared_ptr<MockServicePluginInterface> mockservice;
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
  Admin(std::shared_ptr<EventClientBase>) {}
  ~Admin() {}
  InterfaceMap_t create_interfaces(std::shared_ptr<CoreContext>);
  static const uint core_version{CORE_VERSION};
};