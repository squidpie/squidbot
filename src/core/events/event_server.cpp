/*
event_server.cpp
Copyright (C) 2023  Squidpie
 */

#include <algorithm>
#include <cstdint>
#include <unistd.h>

#include "event_server.h"

void EventServerRunAction::run_action() {
  auto disconnect_id = NULL_CLIENT_ID;
  auto is_disconnect{false};
  std::vector<clientid_t> disconnect_ids;

  std::lock_guard<std::mutex> rguard(*rlock);
  std::for_each(
      clients->begin(), clients->end(), [this, &is_disconnect](auto entry) {
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
          is_disconnect = true;
          q->pop();

        } else if (front_event.source == id) {
          q->pop();
          route_event(front_event);
        }
      });

  if (is_disconnect) {
    for (const clientid_t &id : disconnect_ids) {
      clients->erase(id);
      for (auto entry = subs->begin(); entry != subs->end();) {
        auto found = std::find(entry->second.begin(), entry->second.end(), id);
        if (found != entry->second.end()) {
          entry->second.erase(found);
          entry++;
        } else
          ++entry;
      }
    }
    clients->erase(disconnect_id);
  }
}

void EventServerRunAction::route_event(Event tx) {
  auto event_subs = subs->at(tx.type);
  std::for_each(event_subs.begin(), event_subs.end(), [this, tx](auto id) {
    if (is_valid_route_event(id, tx))
      send_to_client(id, tx);
  });
}

void EventServerRunAction::send_to_client(clientid_t id, Event tx) {
  auto q = clients->at(id).second;
  q->push(tx);
}

bool EventServerRunAction::is_valid_route_event(clientid_t id,
                                                Event route_event) {
  return (id != route_event.source && clients->find(id) != clients->end());
}

std::shared_ptr<EventClientBase> EventServer::create_client() {
  std::lock_guard<std::mutex> rguard(*rlock);
  auto id = get_id();
  auto qlock = std::make_shared<std::mutex>();
  auto q = std::make_shared<std::queue<Event>>();
  register_q(id, qlock, q);
  return std::dynamic_pointer_cast<EventClientBase>(
      std::make_shared<EventClient>(id, qlock, q));
}

void EventServer::register_q(clientid_t id, std::shared_ptr<std::mutex> qlock,
                             std::shared_ptr<std::queue<Event>> q) {
  clients->insert({id, std::make_pair(qlock, q)});
}

clientid_t EventServer::get_id() {
  if (++current_id == 0)
    ++current_id;
  while (clients->find(current_id) != clients->end()) {
    ++current_id;
  }
  return current_id;
}