#pragma once

#include "servicelib.h"

class ServiceLoader {
public:
  static constexpr std::string_view lib = "/services";
  typedef create_service_t create_t;
  ~ServiceLoader() {}
};