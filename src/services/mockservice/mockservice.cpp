#include "mockservice.h"

void MockService::load() {}
void MockService::unload() {}
void MockService::run() {
    PLOGD << "Mock Service Running";
}

// Factory
extern "C" MockService* create(Context_t* context) {
  auto p = new MockService(context);
  return p;
}

extern "C" void destroy(MockService * p) {
  delete p;
}