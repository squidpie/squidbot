/*
factory.cpp
Copyright (C) 2023  Squidpie
 */

#include "templatename/templatename.h"

extern "C" void create(std::string lib_path, std::shared_ptr<Context> context) {
  PLOGD << "TemplateName Create";
  auto service_context = std::dynamic_pointer_cast<CoreContext>(context);
  auto service = std::make_shared<Service<TemplateName>>(service_context);
  service_context->service_manager->register_service<TemplateName>(
      lib_path, std::dynamic_pointer_cast<ServiceBase>(service));
  service->start();
}
