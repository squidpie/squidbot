#include <algorithm>
#include <cstdint>
#include <unistd.h>

#include "event_server.h"

void EventServerRunAction::run_action() {
  std::for_each(clients->begin(), clients->end(), [this](auto entry) {
    auto id = entry.first;
    auto qlock = entry.second.first;
    auto q = entry.second.second;

    const std::lock_guard<std::mutex> _lock(*qlock);
    auto front_event = q->empty() ? NULL_EVENT : q->front();

    if (front_event.type == EVENTS.NULL_EVENT_TYPE) {
      return;
    } else if (front_event.type == SERVEREVENTS.SUBSCRIBE_EVENT_TYPE) {
      q->pop();
      auto data =
          std::dynamic_pointer_cast<SubEventData>(front_event.data)->data;
      (*subs)[data].push_back(id);

    } else if (front_event.type == SERVEREVENTS.DISCONNECT_EVENT_TYPE) {
      q->pop();
      std::cerr << "DISCONNECT GOES HERE" << std::endl;

    } else if (front_event.source == id) {
      q->pop();
      route_event(front_event);
    }
  });
}

void EventServerRunAction::route_event(Event tx) {
  auto event_subs = (*subs)[tx.type];
  std::for_each(event_subs.begin(), event_subs.end(), [this, tx](auto id) {
    if (is_valid_route_event(id, tx))
      send_to_client(id, tx);
  });
}

void EventServerRunAction::send_to_client(uint_fast64_t id, Event tx) {
  auto qlock = (*clients)[id].first;
  auto q = (*clients)[id].second;
  const std::lock_guard<std::mutex> _lock(*qlock);
  q->push(tx);
}

bool EventServerRunAction::is_valid_route_event(uint_fast64_t id,
                                                Event route_event) {
  return (id != route_event.source && clients->find(id) != clients->end());
}

std::shared_ptr<EventClientBase> EventServer::create_client() {
  auto id = get_id();
  auto qlock = std::make_shared<std::mutex>();
  auto q = std::make_shared<std::queue<Event>>();
  register_q(id, qlock, q);
  return std::dynamic_pointer_cast<EventClientBase>(
      std::make_shared<EventClient>(id, qlock, q));
}

void EventServer::register_q(uint_fast64_t id,
                             std::shared_ptr<std::mutex> qlock,
                             std::shared_ptr<std::queue<Event>> q) {
  clients[id] = std::make_pair(qlock, q);
}