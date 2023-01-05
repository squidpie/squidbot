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
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#include "event_server.h"
#include "events.h"
#include "logging.h"
#include "runner.h"
#include "service_loader.h"
#include "service_manager.h"
#include "servicelib.h"

class ServiceInterfaceBase {
public:
  ServiceInterfaceBase(...) {}
  virtual ~ServiceInterfaceBase() {}
};

class ServiceDataBase {
public:
  ServiceDataBase() {}
  virtual ~ServiceDataBase() {}
};

class ServiceBase {
public:
  typedef RunActionBase run_action_t;
  typedef ServiceDataBase data_t;
  typedef ServiceInterfaceBase plugin_interface_t;
  typedef ServiceInterfaceBase external_interface_t;
  ServiceBase() {}
  virtual ~ServiceBase() {}

  virtual void start() = 0;
  virtual void stop() = 0;
  virtual std::shared_ptr<ServiceInterfaceBase> get_interface() = 0;
};

template <class S> class Service : virtual public ServiceBase, std::enable_shared_from_this<Service<S>> {
public:
  typedef typename S::run_action_t R;
  typedef typename R::context_t Rc;
  typedef typename S::data_t D;
  typedef typename S::plugin_interface_t Pi;
  typedef typename S::external_interface_t Ei;

  Service() {}
  Service(std::shared_ptr<ServiceContext> context) {
    PLOGD << "Service Constructor";
    auto client = context->event_server->create_client();
    client->subscribe(TEST_EVENT_TYPE);
    runner.reset(new Runner<R>(std::make_shared<Rc>(client, data)));
  }
  ~Service() {
    stop();
  }
/*
  template <class Sprime>
  Service<Sprime> operator=(const Service<Sprime> &right) {
    Service<Sprime> left{};
    left->inject(right.runner, right.data);
    return left;
  }
*/
  void start() { runner->start(); }
  void stop() { runner->stop(); }

  std::shared_ptr<ServiceInterfaceBase> get_interface() {
    std::cerr << "DEBUG OUTPUT :: Service Template creating interface";
    return std::make_shared<Pi>(data);
    //return std::dynamic_pointer_cast<ServiceInterfaceBase>(std::make_shared<Pi>(data));
  }

  void inject(std::shared_ptr<Runner<R>> _runner, std::shared_ptr<D> _data) {
    runner = _runner;
    data = _data;
  }

protected:
  std::shared_ptr<Runner<R>> runner;
  std::shared_ptr<D> data;
  std::shared_ptr<Ei> api;
};