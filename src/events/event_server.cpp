#include <algorithm>
#include <cstdint>
#include <mutex>
#include <unistd.h>

#include "events.h"

EventClientBase* EventServer::create_client() {
  auto id = get_id();
  auto qlock = new std::mutex;
  auto q = new std::queue<Event>;
  register_q(id, qlock, q);
  return dynamic_cast<EventClientBase*>(new EventClient(id, qlock, q));
}

void EventServer::register_q(uint_fast64_t id, std::mutex* qlock, std::queue<Event>* q) {
  clients[id] = std::make_pair(qlock, q);
}

void EventServer::run() {
  is_running = true;
  while(is_running) {
    std::for_each(clients.begin(), clients.end(), [this](auto entry) {
      auto id = entry.first;
      auto qlock = entry.second.first;
      auto q = entry.second.second;
      qlock->lock();
      auto front_event = q->front();
      if (front_event.source != id) { qlock->unlock(); return; }
      switch(front_event.type) {
          case SUBSCRIBE_EVENT_TYPE:
            q->pop();
            qlock->unlock();
            subscriptions[front_event.data].push_back(id);
            break;
          default:
            q->pop();
            qlock->unlock();
            route_event(front_event);
      }
        });


  }
}

void EventServer::route_event(Event tx) {
  auto subs = subscriptions[tx.type];
  std::for_each(subs.begin(), subs.end(), [this, tx](auto id) {
    if (id == tx.source) return;
    if (clients.find(id) == clients.end()) return;

    auto qlock = clients[id].first;
    auto q = clients[id].second;
    qlock->lock();
    q->push(tx);
    qlock->unlock();
  });
}