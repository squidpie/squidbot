/*
factory.cpp
Copyright (C) 2023  Squidpie
 */

#include "adminlisten.h"

extern "C" void create(std::string lib_path, std::shared_ptr<Context> context) {
  PLOGD << "AdminListen Create";
  auto service_context = std::dynamic_pointer_cast<CoreContext>(context);
  auto service = std::make_shared<Service<AdminListen>>(service_context);
  service_context->service_manager->register_service<AdminListen>(
      lib_path, std::dynamic_pointer_cast<ServiceBase>(service));
  service->start();
}