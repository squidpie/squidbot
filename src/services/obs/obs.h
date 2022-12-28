#pragma once

#include "service.h"
#include "events.h"

class OBS: virtual public ServiceBase {
public:
  OBS(Context_t* context) : ServiceBase(context) { load(); }
  ~OBS() { unload(); }
  void run();

protected:
  void load();
  void unload();

  //EventBufferClient* buffer_client;
  //bool is_running;
};