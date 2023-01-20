/*
loggin.cpp
Copyright (C) 2023  Squidpie
 */

#include "logging.h"

namespace fs = std::filesystem;

void plog_shared_init(plog::Severity severity, plog::IAppender *plog_appender,
                     std::string dir) {
  dir += "/plog";
  if (!fs::exists(dir)) {
    LOGE << "Plog Dir " << dir << " Not Found";
    return;
  }

  for (const auto &entry : fs::directory_iterator(dir)) {
    const auto lib = entry.path().filename().string();
    if (lib == "libploginit.so") {
      const auto lib_path = dir + "/" + lib;
      LOGD << "loading plog_init from " << lib_path;

      void *handle = dlopen(lib_path.c_str(), RTLD_LAZY);
      const char *dlsym_error = dlerror();
      if (dlsym_error) {
        LOGE << "Failed to open logger lib";
      }

      auto plog_init = (plog_init_t)dlsym(handle, "plog_init");
      dlsym_error = dlerror();
      if (dlsym_error) {
        LOGE << "Failed to link plog_init function";
      }

      try {
        plog_init(severity, plog_appender);
      } catch (const std::runtime_error &err) {
        LOGE << "Failed to execute init function";
      }

      dlclose(handle);
    }
  }
}