#pragma once

#include "service.h"

class MockService: virtual public ServiceBase {
public:
  MockService(Context_t* context) : ServiceBase(context) { load(); }
  ~MockService() { unload(); }
  void run();

protected:
  void load();
  void unload();
};