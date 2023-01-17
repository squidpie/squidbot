#pragma once
#include <cstdint>
#include <memory>

#include "utils/uuid.h"

const std::string EVENTNAMESPACE = "e4f3518e-9626-11ed-8ff0-00155d30393f";

struct DefaultEvents {
  uuid_t NULL_EVENT_TYPE = 0;
  uuid_t TEST_EVENT_TYPE = uuid(EVENTNAMESPACE + "TEST_EVENT_TYPE");
};

static inline DefaultEvents EVENTS;

const uint_fast64_t NULL_CLIENT_ID = 0;

class EventDataBase {
public:
  virtual ~EventDataBase() {}
};

class Event {
public:
  uint_fast64_t source;
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