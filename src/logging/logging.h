/*
logging.h
Copyright (C) 2023  Squidpie
 */

#pragma once

#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>
#include <plog/Severity.h>
#define PLOG_LOCAL

#include <dlfcn.h>
#include <filesystem>

typedef void (*plog_init_t)(plog::Severity, plog::IAppender *);

void plog_shared_init(plog::Severity, plog::IAppender *, std::string);