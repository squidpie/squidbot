#pragma once

#include <cstdint>
#include <mutex>
#include <string>
#include "plog/Appenders/IAppender.h"

#include "events.h"

const std::string LIB_DIR = "/home/squidpie/Repos/squidbot/targets/lib/";

typedef struct Context {
  plog::IAppender* plog_appender;
  EventServer* buffer_server;
} Context_t;