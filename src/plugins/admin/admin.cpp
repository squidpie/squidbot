#include "admin.h"

AdminAction::AdminAction(InterfaceMap_t *_interfaces,
                         std::shared_ptr<PluginDataBase>)
    : interfaces(_interfaces) {
  mockservice = std::dynamic_pointer_cast<MockServicePluginInterface>(
      interfaces->at(std::type_index(typeid(MockService))));
}

bool AdminFilter::is_trigger(Event rx) {
  return (rx.type == EVENTS.TEST_EVENT_TYPE);
}

void AdminAction::trigger_action(Event rx) { mockservice->test(); }

/*
create_interfaces : load the interface map with all interfaces the
trigger_action will require to interact with
 */
InterfaceMap_t Admin::create_interfaces(std::shared_ptr<CoreContext> context) {
  InterfaceMap_t interfaces;
  interfaces[std::type_index(typeid(MockService))] =
      context->service_manager->get_interface<MockService>();
  return interfaces;
}
