#include "mockservice.h"

void MockService::load() {}
void MockService::unload() {}
bool MockService::run() { return true; }

// Factory
extern "C" MockService* create() {
  auto p = new MockService;
  return p;
}

extern "C" void destroy(MockService * p) {
  delete p;
}