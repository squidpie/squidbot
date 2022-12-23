#include "service.h"

bool ServiceContainer::run() {
  if (is_service_loaded()) throw ServiceRuntimeError();
  return service->run();
}

void ServiceContainer::gen_lib_path(std::string dir) {
  lib_path = dir + "/lib" + name + ".so";
}

void ServiceContainer::init() {
  void* handle = dlopen(lib_path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
  const char* dlsym_error = dlerror();
  if (!handle) {
    throw ServiceLibError();
  }

  create_service = (create_t*) dlsym(handle, "create");
  dlsym_error = dlerror();
  if (dlsym_error) {
    throw ServiceLibError();
  }

  destroy_service = (destroy_t*) dlsym(handle, "destroy");
  dlsym_error = dlerror();
  if (dlsym_error) {
    throw ServiceLibError();
  }

}

void ServiceContainer::create() {
  service = create_service();
  if (is_service_loaded()) {
    throw ServiceRuntimeError();
  }
}

void ServiceContainer::destroy() {
  destroy_service(service);
  service = NULL;
}