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
    clientid_t,
    std::pair<std::shared_ptr<std::mutex>, std::shared_ptr<std::queue<Event>>>>
    ClientMap_t;
typedef std::unordered_map<uuid_t, std::vector<clientid_t>> SubMap_t;

class EventServerRunActionContext : virtual public RunActionContextBase {
public:
  EventServerRunActionContext(std::shared_ptr<ClientMap_t> clients,
                              std::shared_ptr<SubMap_t> subs,
                                  std::shared_ptr<std::mutex> rlock)
      : clients(clients), subs(subs), rlock(rlock) {}
  ~EventServerRunActionContext() {}
  std::shared_ptr<ClientMap_t> clients;
  std::shared_ptr<SubMap_t> subs;
  std::shared_ptr<std::mutex> rlock;
};

class EventServerRunAction : virtual public RunActionBase {
public:
  EventServerRunAction(std::shared_ptr<EventServerRunActionContext> _context)
      : clients(_context->clients), subs(_context->subs), rlock(_context->rlock) {}
  ~EventServerRunAction() {}
  void run_action();
  typedef EventServerRunActionContext context_t;

protected:
  void route_event(Event);
  bool is_valid_route_event(clientid_t, Event);
  void send_to_client(clientid_t, Event);
  void destroy_client(clientid_t, std::shared_ptr<std::mutex>);
  void debug_dump(clientid_t id, std::shared_ptr<std::mutex> qlock,
                  std::shared_ptr<std::queue<Event>> q) {
    std::cerr << ":: EventServer Dump ::" << std::endl;
    std::cerr << ":: current id = " << id << std::endl;
    std::cerr << ":: current qlock = " << qlock << std::endl;
    std::cerr << ":: current q = " << q << std::endl;
    std::cerr << ":: End Dump ::" << std::endl;
  }
  std::shared_ptr<ClientMap_t> clients;
  std::shared_ptr<SubMap_t> subs;
  std::shared_ptr<std::mutex> rlock;
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
    auto run_context =
        std::make_shared<EventServerRunActionContext>(clients, subscriptions, rlock);
    runner = std::make_shared<EventServerRunner_t>(run_context);
    assert(runner != NULL);
  }
  ~EventServer() { stop(); }
  std::shared_ptr<EventClientBase> create_client();
  void start() { runner->start(); };
  void stop() { runner->stop(); }

#ifdef _GTEST_COMPILE
  auto inject(auto _clients, auto _subs) {
    clients = _clients;
    subscriptions = _subs;
  }
  auto dump() { return std::make_pair(clients, subscriptions); }
#endif

protected:
  void register_q(clientid_t, std::shared_ptr<std::mutex>,
                  std::shared_ptr<std::queue<Event>>);

  clientid_t get_id();

  bool is_running = false;
  std::shared_ptr<std::mutex> rlock = std::make_shared<std::mutex>();
  std::shared_ptr<ClientMap_t> clients = std::make_shared<ClientMap_t>();
  std::shared_ptr<SubMap_t> subscriptions = std::make_shared<SubMap_t>();

  clientid_t current_id = 0;

  std::shared_ptr<EventServerRunner_t> runner;
};