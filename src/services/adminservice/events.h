/*
events.h
Copyright (C) 2023  Squidpie
 */

#pragma once

#include "utils/uuid.h"

const std::string ADMINSERVICENAMESPACE = "";

struct AdminServiceEvents {
  uuid_t ADMIN_UPDATE_EVENT_TYPE = uuid(ADMINSERVICENAMESPACE + "ADMIN_UPDATE_EVENT_TYPE");
};

static inline AdminServiceEvents ADMINSERVICEEVENTS;