#pragma once
#include <initializer_list>
#include <stdarg.h>
#include <typeindex>
#include <unordered_map>

#include "event_client.h"
#include "event_server.h"
#include "events.h"
#include "lib/core_context.h"
#include "runner.h"
#include "service.h"

typedef std::unordered_map<std::type_index,
                           std::shared_ptr<ServiceInterfaceBase>>
    InterfaceMap_t;

class PluginActionBase {
public:
  PluginActionBase() {}
  virtual ~PluginActionBase() {}
  virtual void trigger_action(Event) = 0;
};

class PluginDataBase {
public:
  PluginDataBase() {}
  virtual ~PluginDataBase() {}
};

class PluginFilterBase {
public:
  PluginFilterBase() {}
  virtual ~PluginFilterBase() {}
  virtual bool is_trigger(Event) = 0;
};

class PluginRunActionContext : virtual public RunActionContextBase {
public:
  PluginRunActionContext(std::shared_ptr<EventClientBase> client,
                         std::shared_ptr<PluginFilterBase> filter,
                         std::shared_ptr<PluginActionBase> action)
      : filter(filter), client(client), action(action) {}
  ~PluginRunActionContext() {}
  std::shared_ptr<PluginFilterBase> filter;
  std::shared_ptr<EventClientBase> client;
  std::shared_ptr<PluginActionBase> action;
};

class PluginRunAction : virtual public RunActionBase {
public:
  typedef PluginRunActionContext context_t;
  PluginRunAction(std::shared_ptr<PluginRunActionContext> context)
      : client(context->client), filter(context->filter),
        action(context->action) {}
  ~PluginRunAction() {}

  void run_action() override;

protected:
  std::shared_ptr<EventClientBase> client;
  std::shared_ptr<PluginFilterBase> filter;
  std::shared_ptr<PluginActionBase> action;
};

class PluginBase {
public:
  PluginBase() {}
  virtual ~PluginBase() {}
  virtual void start() = 0;
  virtual void stop() = 0;
  virtual void reload() = 0;
  virtual bool is_dependent(std::type_index) = 0;
};

template <class P> class Plugin : public virtual PluginBase {
public:
  typedef typename P::filter_t Pf;
  typedef typename P::action_t Pa;

  Plugin(std::shared_ptr<CoreContext> context) : context(context) {
    auto client = configure_client();
    configure_plugin(client);
    configure_runner(client);
  }
  Plugin() {}
  ~Plugin() { runner->stop(); }

  void start() override { runner->start(); }
  void stop() override { runner->stop(); }
  void reload() override { std::cerr << "RELOADING PLUGIN" << std::endl; }
  bool is_dependent(std::type_index index) override {
    return (interfaces.find(index) != interfaces.end());
  }

#ifdef _GTEST_COMPILE
  void inject(std::shared_ptr<RunnerBase> _runner, InterfaceMap_t _interfaces) {
    runner = _runner;
    interfaces = _interfaces;
  }
#endif

protected:
  std::shared_ptr<CoreContext> context;
  std::shared_ptr<RunnerBase> runner;
  std::shared_ptr<PluginFilterBase> filter;
  std::shared_ptr<PluginDataBase> data;
  std::shared_ptr<PluginActionBase> action;
  InterfaceMap_t interfaces;

  auto configure_client() {
    auto client = context->event_server->create_client();
    client->subscribe(TEST_EVENT_TYPE);
    return client;
  }

  void configure_plugin(auto client) {
    auto plugin = std::make_unique<P>(client);
    interfaces = plugin->create_interfaces(context);
  }

  void configure_runner(auto client) {
    action = std::make_shared<Pa>(&interfaces, data);
    filter = std::make_shared<Pf>();

    auto plugin_context =
        std::make_shared<PluginRunActionContext>(client, filter, action);

    runner = std::make_shared<Runner<PluginRunAction>>(plugin_context);
  }
};