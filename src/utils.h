#pragma once

#include "plog/Appenders/IAppender.h"
#include <dlfcn.h>
#include <iostream>
#include <string>
#include <filesystem>

#include "logging.h"

namespace fs = std::filesystem;

const std::string LIB_DIR = "/home/squidpie/Repos/squidbot/targets/lib/";
const std::string TEST_LIB_DIR = "targets/lib/test";

class LoadLibError : public std::system_error {};

class Context {
  public:
    Context(std::string lib_dir) : lib_dir(lib_dir) {}
  virtual ~Context() {}
  std::string lib_dir;
};

template <class L> class LibLoader  {
public:

  typedef typename L::create_t create_t;

  LibLoader(std::shared_ptr<Context> context) : context(context) {
    load_all();
  }
  ~LibLoader() {}

  void load_all() {
    for (const auto& entry : fs::directory_iterator(context->lib_dir)) {
      const auto path = entry.path().filename().string();
      if (path.ends_with(".so")) {
        load_lib(context->lib_dir + "/" + path);
      }
    }
  }

  void load_lib(std::string lib_path) {
    PLOGD << "loading lib " << lib_path;
    void *handle = dlopen(lib_path.c_str(), RTLD_LAZY);

    const char *dlsym_error = dlerror();
    if (!handle) {
      LOGE << "Failed to open lib_path: " << lib_path.c_str();
      throw LoadLibError();
    }

    auto create = (create_t)dlsym(handle, "create");

    dlsym_error = dlerror();
    if (dlsym_error) {
      LOGE << "Failed to link create function";
      throw LoadLibError();
    }
    create(context);
    /*
    auto destroy = (destroy_t *)dlsym(handle, "destroy");

    dlsym_error = dlerror();
    if (dlsym_error) {
      LOGE << "Failed to link destroy function";
      throw LoadLibError();
    }
    */
  }

  protected:
    std::shared_ptr<Context> context;
};