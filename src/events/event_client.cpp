#include "events.h"

const Event EventClient::receive() {
  qlock->lock();
  Event event = q->size() ? q->front() : NULL_EVENT;

  if ((event != NULL_EVENT) && (event.source != guid)) {
    q->pop();
  } else {
    event = NULL_EVENT;
  }
  
  qlock->unlock();
  return event;
}

void EventClient::send(Event tx_event) {
  tx_event.source = guid;
  qlock->lock();
  q->push(tx_event);
  qlock->unlock();
}

void EventClient::subscribe(EventType sub_event) {
  Event tx = { guid, SUBSCRIBE_EVENT_TYPE, sub_event };
  send(tx);
}