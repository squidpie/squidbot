#include "service.h"
#include "utils.h"

void ServiceContainer::run() {
  need_service();
  service->run();
}

void ServiceContainer::gen_lib_path(std::string dir) {
  lib_path = dir + "/lib" + name + ".so";
}

void ServiceContainer::init() {
  void* handle = dlopen(lib_path.c_str(), RTLD_LAZY);
  const char* dlsym_error = dlerror();
  if (!handle) {
    LOGE << "Failed to open lib_path: " << lib_path.c_str();
    throw ServiceLibError();
  }

  create_service = (create_t*) dlsym(handle, "create");
  dlsym_error = dlerror();
  if (dlsym_error) {
    LOGE << "Failed to link create function";
    throw ServiceLibError();
  }

  destroy_service = (destroy_t*) dlsym(handle, "destroy");
  dlsym_error = dlerror();
  if (dlsym_error) {
    LOGE << "Failed to link destroy function";
    throw ServiceLibError();
  }

}

void ServiceContainer::create() {
  service = create_service(context);
  PLOGD << "service address = " << service;
  need_service();
}

void ServiceContainer::destroy() {
  destroy_service(service);
  service = NULL;
}

void ServiceContainer::need_service() {
 if (!service) throw ServiceRuntimeError();
}