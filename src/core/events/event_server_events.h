/*
event_server_events.h
Copyright (C) 2023  Squidpie
 */

#pragma once

#include "utils/uuid.h"

const std::string EVENTSERVERNAMESPACE = "96b3ff42-9643-11ed-87d5-00155d30393f";

struct ServerEvents {
  uuid_t SUBSCRIBE_EVENT_TYPE =
      uuid(EVENTSERVERNAMESPACE + "SUBSCRIBE_EVENT_TYPE");
  uuid_t DISCONNECT_EVENT_TYPE =
      uuid(EVENTSERVERNAMESPACE + "DISCONNECT_EVENT_TYPE");
};

static inline ServerEvents SERVEREVENTS;