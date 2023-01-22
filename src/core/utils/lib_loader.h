/*
lib_loader.h
Copyright (C) 2023  Squidpie
 */

#pragma once

#include "dlfcn.h"
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "logging/logging.h"
#include "utils/context.h"

namespace fs = std::filesystem;

class LoadLibError : public std::runtime_error {};

template <class L> class LibLoader {
 public:
  typedef typename L::create_t create_t;

  explicit LibLoader(std::shared_ptr<Context> context) : context(context) {
    load_all(context->lib_dir + std::string(L::lib));
  }
  ~LibLoader() {
    for (const auto &[key, handle] : libs) {
      PLOGD << "Closing handle " << key;
      dlclose(handle);
    }
  }

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

    void *handle = dlopen(lib_path.c_str(), RTLD_NOW | RTLD_DEEPBIND);
    const char *dlsym_error = dlerror();
    if (dlsym_error || handle == nullptr) {
      LOGE << "Failed to open lib_path";
      throw std::runtime_error(dlsym_error);
    }

    auto create = (create_t)dlsym(handle, "create");
    dlsym_error = dlerror();
    if (dlsym_error) {
      LOGE << "Failed to link create function";
      dlclose(handle);
      throw std::runtime_error(dlsym_error);
    }

    try {
      create(lib_path, context);
    } catch (const std::runtime_error &err) {
      LOGE << "Failed to create object";
      dlclose(handle);
      throw std::runtime_error(err.what());
    }
    if (libs.find(lib_path) != libs.end()) {
      auto _handle = libs.at(lib_path);
      dlclose(_handle);
      libs.erase(lib_path);
    }
    libs.insert({lib_path, handle});
  }

 protected:
  std::shared_ptr<Context> context;
  std::unordered_map<std::string, void *> libs;
};
