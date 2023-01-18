#pragma once

#include <cstdint>
#include <memory>
#include <mutex>
#include <queue>

#include "core.h"
#include "event_server_events.h"
#include "events.h"

class EventClientBase {
public:
  virtual ~EventClientBase() {}
  virtual void send(Event) = 0;
  virtual const Event receive() = 0;
  virtual void subscribe(uuid_t) = 0;
};

class EventClient : public EventClientBase {
public:
  EventClient(clientid_t id, std::shared_ptr<std::mutex> qlock,
              std::shared_ptr<std::queue<Event>> q)
      : id(id), qlock(qlock), q(q) {}
  ~EventClient() {
    send(Event{NULL_CLIENT_ID, SERVEREVENTS.DISCONNECT_EVENT_TYPE});
  }
  void send(Event) override;
  const Event receive() override;
  void subscribe(uuid_t) override;

protected:
  Event get_front();
  bool is_front_valid();

private:
  clientid_t id;
  std::shared_ptr<std::mutex> qlock;
  std::shared_ptr<std::queue<Event>> q;
};