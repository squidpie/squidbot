#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <mutex>
#include <queue>
#include <unordered_map>

#include "core.h"
#include "event_client.h"
#include "event_server_events.h"
#include "events.h"
#include "runner.h"

struct SubEventData : virtual EventDataBase {
  SubEventData(uuid_t data) : data(data) {}
  ~SubEventData() {}
  uuid_t data;
};

typedef std::unordered_map<
    uint_fast64_t,
    std::pair<std::shared_ptr<std::mutex>, std::shared_ptr<std::queue<Event>>>>
    ClientMap_t;
typedef std::unordered_map<uuid_t, std::vector<uint_fast64_t>> SubMap_t;

class EventServerRunActionContext : virtual public RunActionContextBase {
public:
  EventServerRunActionContext() {}
  EventServerRunActionContext(std::shared_ptr<ClientMap_t> _clients,
                              std::shared_ptr<SubMap_t> _subs)
      : clients(_clients.get()), subs(_subs.get()) {}
  EventServerRunActionContext(ClientMap_t *_clients, SubMap_t *_subs)
      : clients(_clients), subs(_subs) {}
  ~EventServerRunActionContext() {}
  ClientMap_t *clients;
  SubMap_t *subs;
};

class EventServerRunAction : virtual public RunActionBase {
public:
  EventServerRunAction(std::shared_ptr<EventServerRunActionContext> _context)
      : clients(_context->clients), subs(_context->subs) {}
  ~EventServerRunAction() {}
  void run_action();
  typedef EventServerRunActionContext context_t;

protected:
  void route_event(Event);
  bool is_valid_route_event(uint_fast64_t, Event);
  void send_to_client(uint_fast64_t, Event);
  void destroy_client(uint_fast64_t, std::shared_ptr<std::mutex>);
  void debug_dump(uint_fast64_t id, std::shared_ptr<std::mutex> qlock,
                  std::shared_ptr<std::queue<Event>> q) {
    std::cerr << ":: EventServer Dump ::" << std::endl;
    std::cerr << ":: current id = " << id << std::endl;
    std::cerr << ":: current qlock = " << qlock << std::endl;
    std::cerr << ":: current q = " << q << std::endl;
    std::cerr << ":: End Dump ::" << std::endl;
  }
  ClientMap_t *clients;
  SubMap_t *subs;
  std::thread dthread;
};

typedef Runner<EventServerRunAction> EventServerRunner_t;

class EventServerBase {
public:
  virtual ~EventServerBase() {}
  virtual std::shared_ptr<EventClientBase> create_client() = 0;
};

class EventServer : public EventServerBase {
public:
  EventServer(std::shared_ptr<EventServerRunner_t> _runner) {
    assert(_runner != NULL);
    runner = _runner;
  }
  EventServer() {
    runner = std::make_shared<EventServerRunner_t>(
        std::make_shared<EventServerRunActionContext>(&clients,
                                                      &subscriptions));
    assert(runner != NULL);
  }
  ~EventServer() { stop(); }
  std::shared_ptr<EventClientBase> create_client();
  void start() { runner->start(); };
  void stop() { runner->stop(); }

#ifdef _GTEST_COMPILE
  auto dump() { return std::make_pair(clients, subscriptions); }
#endif

protected:
  void register_q(uint_fast64_t, std::shared_ptr<std::mutex>,
                  std::shared_ptr<std::queue<Event>>);

  uint_fast64_t get_id();

  bool is_running = false;
  ClientMap_t clients;
  SubMap_t subscriptions;

  uint_fast64_t current_id = 0;

  std::shared_ptr<EventServerRunner_t> runner;
};