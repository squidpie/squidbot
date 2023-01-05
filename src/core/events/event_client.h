#pragma once

#include <cstdint>
#include <memory>
#include <mutex>
#include <queue>

#include "events.h"

class EventClientBase {
public:
  virtual ~EventClientBase() {}
  virtual void send(Event) = 0;
  virtual const Event receive() = 0;
  virtual void subscribe(EventType) = 0;
};

class EventClient : public EventClientBase {
public:
  EventClient(uint_fast64_t id, std::shared_ptr<std::mutex> qlock, std::shared_ptr<std::queue<Event>> q)
      : id(id), qlock(qlock), q(q) {}
  ~EventClient() {}
  void send(Event);
  const Event receive();
  void subscribe(EventType);

  protected:
    Event get_front();
    bool is_front_valid();

private:
  uint_fast64_t id;
  std::shared_ptr<std::mutex> qlock;
  std::shared_ptr<std::queue<Event>> q;
};