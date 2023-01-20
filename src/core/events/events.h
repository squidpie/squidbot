/*
events.h
Copyright (C) 2023  Squidpie
 */

#pragma once
#include <cstdint>
#include <memory>

#include "core.h"
#include "utils/uuid.h"

const std::string EVENTNAMESPACE = "e4f3518e-9626-11ed-8ff0-00155d30393f";

struct DefaultEvents {
  uuid_t NULL_EVENT_TYPE = 0;
  uuid_t TEST_EVENT_TYPE = uuid(EVENTNAMESPACE + "TEST_EVENT_TYPE");
};

static inline DefaultEvents EVENTS;

struct EventDataBase {
  virtual ~EventDataBase() {}
};

class Event {
public:
  clientid_t source;
  uuid_t type;
  std::shared_ptr<EventDataBase> data;

  bool operator==(const Event e) const {
    return (source == e.source && type == e.type && data == e.data);
  }

  bool operator!=(const Event e) const {
    return (source != e.source || type != e.type || data != e.data);
  }
};

const Event NULL_EVENT = {NULL_CLIENT_ID, EVENTS.NULL_EVENT_TYPE, nullptr};