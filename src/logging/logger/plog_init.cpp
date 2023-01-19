#include <memory>

#include "lib/core_context.h"
#include "logging.h"

// Global Shared Lib PLOG init caller
extern "C" void plog_init(plog::Severity s, plog::IAppender *a) {
  plog::init(s, a);
}