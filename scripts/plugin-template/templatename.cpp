/*
templatename.cpp
Copyright (C) 2023  Squidpie
 */

#include <memory>

#include "templatename/templatename.h"

/*
Define the events filter that will trigger your plugin action
 */
bool TemplateNameFilter::is_trigger(Event rx) {
  return (rx.type == EVENTS.TEST_EVENT_TYPE);
}

/*
Define the actions that will occur when is_trigger returns true
 */
void TemplateNameAction::trigger_action(Event rx) {}

TemplateNameAction::TemplateNameAction(InterfaceMap_t *_interfaces,
                                       std::shared_ptr<PluginDataBase>)
    : interfaces(_interfaces) {
  /* Unload interfaces for Action example
  servicename = std::dynamic_pointer_cast<ServiceNamePluginInterface>(
      interfaces->at(std::type_index(typeid(ServiceName))));
   */
}

/*
create_interfaces : load the interface map with all interfaces the
trigger_action will require to interact with
 */
InterfaceMap_t TemplateName::create_interfaces(std::shared_ptr<CoreContext> context) {
  InterfaceMap_t interfaces;
  /* example interface load
  interfaces.insert({std::type_index(typeid(ServiceName)),
                     context->service_manager->get_interface<ServiceName>(
                         MeowNameListen::service_version)});
  */
  return interfaces;
}
