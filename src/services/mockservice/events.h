#pragma once

#include "utils/uuid.h"

const std::string MOCKSERVICENAMESPACE = "3cf006d2-9629-11ed-9628-00155d30393f";

struct MockServiceEvents {
  uuid_t MOCK_EVENT_TYPE = uuid(MOCKSERVICENAMESPACE + "MOCK_EVENT_TYPE");
};

static inline MockServiceEvents MOCKSERVICEEVENTS;