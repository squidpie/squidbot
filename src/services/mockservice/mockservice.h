#ifndef __MOCK_SERVICE_H__
#define __MOCK_SERVICE_H__

#include "service.h"

class MockService: virtual public ServiceBase {
public:
  MockService() { load(); }
  ~MockService() { unload(); }
  bool run();

protected:
  void load();
  void unload();
};
#endif