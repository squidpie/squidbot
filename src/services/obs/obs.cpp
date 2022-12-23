#include "obs.h"

void OBS::load() {}
void OBS::unload() {}
bool OBS::run() { return true; }

extern "C" OBS* create() {
  return new OBS;
}

extern "C" void destroy(OBS * p) {
  delete p;
}