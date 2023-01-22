/*
admin.cpp
Copyright (C) 2023  Squidpie
 */

#include <memory>

#include "admin/admin.h"

AdminAction::AdminAction(InterfaceMap_t *_interfaces,
                         std::shared_ptr<PluginDataBase>)
    : interfaces(_interfaces) {
  adminlisten = std::dynamic_pointer_cast<AdminListenPluginInterface>(
      interfaces->at(std::type_index(typeid(AdminListen))));
}

bool AdminFilter::is_trigger(Event rx) {
  return (rx.type == EVENTS.TEST_EVENT_TYPE);
}

void AdminAction::trigger_action(Event rx) {}

/*
create_interfaces : load the interface map with all interfaces the
trigger_action will require to interact with
 */
InterfaceMap_t Admin::create_interfaces(std::shared_ptr<CoreContext> context) {
  InterfaceMap_t interfaces;
  interfaces.insert({std::type_index(typeid(AdminListen)),
                     context->service_manager->get_interface<AdminListen>(
                         AdminListen::service_version)});
  return interfaces;
}
