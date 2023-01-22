/*
factory.cpp
Copyright (C) 2023  Squidpie
 */

#include "adminservice.h"
#include <memory>

extern "C" void create(std::string lib_path, std::shared_ptr<Context> context) {
  PLOGD << "AdminService Create";
  auto service_context = std::dynamic_pointer_cast<CoreContext>(context);
  auto service = std::make_shared<Service<AdminService>>(service_context);
}