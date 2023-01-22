/*
service.h
Copyright (C) 2023  Squidpie
 */

#pragma once

#include <memory>

#include "core/core.h"
#include "events/event_server.h"
#include "events/events.h"
#include "lib/core_context.h"
#include "logging/logging.h"

class ServiceInterfaceBase
{
 public:
  ServiceInterfaceBase() {}
  virtual ~ServiceInterfaceBase() {}
};

class ServiceDataBase
{
 public:
  ServiceDataBase() {}
  virtual ~ServiceDataBase() {}
};

class ServiceBase
{
 public:
  ServiceBase() {}
  virtual ~ServiceBase() {}

  virtual void start() = 0;
  virtual void stop() = 0;
  virtual std::shared_ptr<ServiceInterfaceBase> get_interface() = 0;
  virtual const uint core_version() = 0;
};

template <class S> class Service : virtual public ServiceBase
{
 public:
  typedef typename S::run_action_t R;
  typedef typename R::context_t Rc;
  typedef typename S::data_t D;
  typedef typename S::plugin_interface_t Pi;
  typedef typename S::external_interface_t Ei;

  Service() {}
  explicit Service(std::shared_ptr<CoreContext> context)
  {
    auto client = context->event_server->create_client();
    client->subscribe(EVENTS.TEST_EVENT_TYPE);
    runner = std::make_shared<Runner<R>>(std::make_shared<Rc>(client, data));
  }
  ~Service()
  { stop(); }

  void start() override
  { runner->start(); }

  void stop() override
  { runner->stop(); }

  std::shared_ptr<ServiceInterfaceBase> get_interface() override
  { return std::make_shared<Pi>(data); }

  const uint core_version() override
  { return S::core_version; }

#ifdef _GTEST_COMPILE
  void inject(std::shared_ptr<Runner<R>> _runner, std::shared_ptr<D> _data) {
    runner = _runner;
    data = _data;
  }
#endif

 protected:
  std::shared_ptr<Runner<R>> runner;
  std::shared_ptr<D> data;
  std::shared_ptr<Ei> api;
};
