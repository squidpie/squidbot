/*
events.h
Copyright (C) 2023  Squidpie
 */

#pragma once

#include "utils/uuid.h"

const std::string ADMINLISTENNAMESPACE = "";

struct AdminListenEvents {
  uuid_t ADMIN_UPDATE_EVENT_TYPE = uuid(ADMINLISTENNAMESPACE + "ADMIN_UPDATE_EVENT_TYPE");
};

static inline AdminListenEvents ADMIN_EVENTS;