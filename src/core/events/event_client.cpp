#include "event_client.h"

const Event EventClient::receive() {
  std::lock_guard<std::mutex> _l(*qlock);
  return (is_front_valid()) ? get_front() : NULL_EVENT;
}

bool EventClient::is_front_valid() {
  return (q->size() && q->front().source != id);
}

Event EventClient::get_front() {
  Event e = q->front();
  q->pop();
  return e;
}

void EventClient::send(Event tx_event) {
  std::lock_guard<std::mutex> _l(*qlock);
  tx_event.source = id;
  q->push(tx_event);
}

void EventClient::subscribe(EventType sub_event) {
  Event tx = {SUBSCRIBE_EVENT_TYPE, sub_event};
  send(tx);
}