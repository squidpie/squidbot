/*
events.h
Copyright (C) 2023  Squidpie
 */

#pragma once

#include "utils/uuid.h"

const std::string ADMINLISTEN_NAMESPACE = "";

struct AdminListenEvents {
  uuid_t ADMIN_UPDATE_EVENT_TYPE = uuid(ADMINLISTEN_NAMESPACE + "ADMIN_UPDATE_EVENT_TYPE");
};

static inline AdminListenEvents ADMIN_EVENTS;