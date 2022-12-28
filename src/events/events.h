#pragma once

#include <cstdint>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <iostream>

const uint_fast64_t NULL_GUID = 0;

enum EventType {
  NULL_EVENT_TYPE,
  TEST_EVENT_TYPE,
  SUBSCRIBE_EVENT_TYPE,
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

const Event NULL_EVENT = { NULL_GUID, NULL_EVENT_TYPE, NULL_EVENT_TYPE };

typedef std::unordered_map<uint_fast64_t, std::pair<std::mutex*, std::queue<Event>*>> ClientMap_t;

class EventClient {
  public:
    EventClient(uint_fast64_t guid,
                std::mutex* qlock,
                std::queue<Event>* q) :
                guid(guid), qlock(qlock), q(q) {}
    void send(Event tx_event);
    const Event receive();
    void subscribe(EventType);
  private:
    uint_fast64_t guid;
    std::mutex* qlock;
    std::queue<Event>* q;

};

class EventServer {
public:
  EventClient* create_client();
  void run();
  void stop() { is_running = false; }

protected:
  void register_q(uint_fast64_t, std::mutex*, std::queue<Event>*);
  void route_event(Event);

  uint_fast64_t get_id() {
    return ++current_id;
  }

  void debug_dump(uint_fast64_t id, std::mutex* qlock, std::queue<Event>* q) {
    std::cerr << ":: EventServer Dump ::" << std::endl;
    std::cerr << ":: current id = " << id << std::endl;
    std::cerr << ":: current qlock = " << qlock << std::endl;
    std::cerr << ":: current q = " << q << std::endl;
    std::cerr << ":: clients = " << &clients << std::endl;
    std::cerr << ":: subscriptions = " << &subscriptions << std::endl;
    std::cerr << ":: End Dump ::" << std::endl;
  }

  bool is_running = false;
  ClientMap_t clients;
  std::unordered_map<EventType, std::vector<uint_fast64_t>> subscriptions;

  uint_fast64_t current_id = 0;
};