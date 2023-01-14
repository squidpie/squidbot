#pragma once

#include "plugin.h"
#include "mockservice/mockservice.h"

class MockPluginAction : virtual public PluginActionBase {
public:
  MockPluginAction(InterfaceMap_t *interfaces, std::shared_ptr<PluginDataBase>);
  ~MockPluginAction() {}
  void trigger_action(Event) override;
  protected:
    InterfaceMap_t *interfaces;
    std::shared_ptr<MockServicePluginInterface> mockservice;
};

class MockPluginFilter : virtual public PluginFilterBase {
public:
  ~MockPluginFilter() {}
  bool is_trigger(Event) override;
};

class MockPlugin {
public:
  typedef MockPluginAction action_t;
  typedef MockPluginFilter filter_t;
  MockPlugin(std::shared_ptr<EventClientBase>) {}
  ~MockPlugin() {}
  InterfaceMap_t create_interfaces(std::shared_ptr<CoreContext>);
};