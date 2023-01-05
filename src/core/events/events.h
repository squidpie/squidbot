#pragma once
#include <cstdint>

const uint_fast64_t NULL_GUID = 0;

enum EventType {
  NULL_EVENT_TYPE = 0,
  TEST_EVENT_TYPE = 1,
  SUBSCRIBE_EVENT_TYPE = 2,
};

class Event {
public:
  uint_fast64_t source;
  EventType type;
  EventType data;

  bool operator==(const Event e) const {
    return (source == e.source && type == e.type && data == e.data);
  }

  bool operator!=(const Event e) const {
    return (source != e.source || type != e.type || data != e.data);
  }
};

const Event NULL_EVENT = {NULL_GUID, NULL_EVENT_TYPE, NULL_EVENT_TYPE};