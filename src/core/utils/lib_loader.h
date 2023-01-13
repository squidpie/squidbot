#pragma once

#include "plog/Appenders/IAppender.h"
#include <dlfcn.h>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#include "logging.h"
#include "utils/context.h"

namespace fs = std::filesystem;

class LoadLibError : public std::runtime_error {};

template <class L> class LibLoader {
public:
  typedef typename L::create_t create_t;

  LibLoader(std::shared_ptr<Context> context) : context(context) {
    load_all(context->lib_dir + std::string(L::lib));
  }
  ~LibLoader() {}

  void load_all(std::string dir) {
    if (!fs::exists(dir)) {
      std::cerr << "load target " << dir << " does not exist" << std::endl;
      assert(fs::exists(dir));
    }
    for (const auto &entry : fs::directory_iterator(dir)) {
      const auto lib = entry.path().filename().string();
      if (lib.ends_with(".so")) {
        try {
          load_lib(dir + "/" + lib);
        } catch (const std::runtime_error &err) {
          LOGE << "Failed to load " << lib << " with error [" << err.what()
               << "]";
        }
      }
    }
  }

  void load_lib(std::string lib_path) {
    PLOGD << "loading lib " << lib_path;

    void *handle = dlopen(lib_path.c_str(), RTLD_LAZY);
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
      LOGE << "Failed to open lib_path";
      throw std::runtime_error(dlsym_error);
    }

    auto create = (create_t)dlsym(handle, "create");
    dlsym_error = dlerror();
    if (dlsym_error) {
      LOGE << "Failed to link create function";
      throw std::runtime_error(dlsym_error);
    }

    try {
      create(lib_path, context);
    } catch (const std::runtime_error &err) {
      LOGE << "Failed to create object";
      throw std::runtime_error(err.what());
    }

    dlclose(handle);
  }

protected:
  std::shared_ptr<Context> context;
};