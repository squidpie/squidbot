#pragma once

#include <chrono>
#include <cstdint>
#include <dlfcn.h>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <system_error>
#include <thread>

#include "event_server.h"
#include "events.h"
#include "logging.h"
#include "runner.h"

struct ServiceContext {
  plog::IAppender *plog_appender;
  EventServerBase *event_server;
};

class ServiceRuntimeError : public std::runtime_error {
public:
  ServiceRuntimeError() : std::runtime_error("ServiceRuntimeError") {}
};
class ServiceLibError : public std::system_error {};

template <class R, class Rc, class D> class Service;

class ServiceDataBase {
public:
  ServiceDataBase() {}
  virtual ~ServiceDataBase() {}
  virtual void _init() = 0;
};

class ServiceBase {
public:
  ServiceBase() {}
  virtual ~ServiceBase() {}

  virtual void start() = 0;
  virtual void stop() = 0;
};

template <class R, class Rc, class D>
class Service : virtual public ServiceBase {
public:
  Service() {}
  Service(ServiceContext context) {
    PLOGD << "Service Constructor";
    configure_event_client(context);
    configure_runner();
  }
  ~Service() {}

  template <class Rprime, class Rcprime, class Dprime>
  Service<Rprime, Rcprime, Dprime>
  operator=(const Service<Rprime, Rcprime, Dprime> &right) {
    Service<Rprime, Rcprime, Dprime> left{};
    left->inject(right.runner, right.runner_context, right.data);
    return left;
  }

  void start() { runner->start(); }
  void stop() { runner->stop(); }

  void inject(std::shared_ptr<Runner<R, Rc>> _runner,
              std::shared_ptr<Rc> _run_action_context,
              std::shared_ptr<D> _data) {
    runner = _runner;
    runner_context = *_run_action_context;
    data = _data;
  }

protected:
  Rc runner_context;
  std::shared_ptr<Runner<R, Rc>> runner;
  std::shared_ptr<D> data;

private:
  void configure_event_client(ServiceContext context) {
    runner_context.event_client = context.event_server->create_client();
    assert(runner_context.event_client != NULL);
    runner_context.event_client->subscribe(TEST_EVENT_TYPE);
  }

  void configure_runner() {
    runner.reset(new Runner<R, Rc>(std::make_shared<Rc>(runner_context)));
  }
};

typedef ServiceBase *create_service_t(ServiceContext *);
typedef void destroy_service_t(ServiceBase *);