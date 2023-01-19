#pragma once

#include <gmock/gmock.h>

#include "core.h"
#include "corelib.h"

class MockCoreRunActionContext : virtual public RunActionContextBase {
public:
  ~MockCoreRunActionContext() {}
};

class MockCoreRunAction : virtual public RunActionBase {
public:
  typedef MockCoreRunActionContext context_t;
  MockCoreRunAction(std::shared_ptr<MockCoreRunActionContext> __c) {}
  ~MockCoreRunAction() {}
  MOCK_METHOD(void, run_action, (), (override));
};

class MockCoreRunner : virtual public RunnerBase {
public:
  MockCoreRunner() {}
  ~MockCoreRunner() {}
  MOCK_METHOD(void, start, (), (override));
  MOCK_METHOD(void, stop, (), (override));
};

class MockCoreEventClient : public EventClientBase {
public:
  ~MockCoreEventClient() {}
  MOCK_METHOD(void, subscribe, (uuid_t), (override));
  MOCK_METHOD(void, send, (Event), (override));
  MOCK_METHOD(const Event, receive, (), (override));
};

class MockCoreEventServer : public EventServerBase {
public:
  ~MockCoreEventServer() {}
  MOCK_METHOD(std::shared_ptr<EventClientBase>, create_client, (), (override));
};

class MockCoreServiceManager : virtual public ServiceManagerBase {
public:
  ~MockCoreServiceManager() {}
  MOCK_METHOD(void, _register_service,
              (std::type_index,
               (std::pair<std::string, std::shared_ptr<ServiceBase>>)),
              (override));
  MOCK_METHOD(void, load, (std::shared_ptr<CoreContext>), (override));
  MOCK_METHOD(void, unload, (), (override));
  MOCK_METHOD(void, load_service, (std::string), (override));
  MOCK_METHOD(std::shared_ptr<ServiceInterfaceBase>, _get_interface,
              (std::type_index), (override));
  MOCK_METHOD(void, _unload, (std::type_index), (override));
  MOCK_METHOD(void, _reload, (std::shared_ptr<std::type_index>), (override));
};

class MockCoreServiceData : virtual public ServiceDataBase {
public:
  ~MockCoreServiceData() {}
};

class MockCorePluginInterface : virtual public ServiceInterfaceBase {
public:
  MockCorePluginInterface(std::shared_ptr<MockCoreServiceData>) {}
  ~MockCorePluginInterface() {}
};

class MockCoreExternalInterface : virtual public ServiceInterfaceBase {
public:
  ~MockCoreExternalInterface() {}
};

class MockCoreMockService {
public:
  typedef MockCoreRunAction run_action_t;
  typedef MockCoreServiceData data_t;
  typedef MockCorePluginInterface plugin_interface_t;
  typedef MockCoreExternalInterface external_interface_t;
  MockCoreMockService() {}
  ~MockCoreMockService() {}
  static const uint service_version{0};
  static const uint core_version{CORE_VERSION};
};

class MockCorePluginAction : virtual public PluginActionBase {
public:
  MockCorePluginAction() {}
  MockCorePluginAction(InterfaceMap_t *, std::shared_ptr<PluginDataBase>) {}
  ~MockCorePluginAction() {}
  MOCK_METHOD(void, trigger_action, (Event), (override));
};

class MockCorePluginFilter : virtual public PluginFilterBase {
public:
  MockCorePluginFilter() {}
  ~MockCorePluginFilter() {}
  MOCK_METHOD(bool, is_trigger, (Event), (override));
};

class MockCorePlugin {
public:
  typedef MockCorePluginAction action_t;
  typedef MockCorePluginFilter filter_t;
  MockCorePlugin() {}
  MockCorePlugin(
      std::shared_ptr<EventClientBase>) { /* <Register Event Subscriptions> */
  }
  ~MockCorePlugin() {}
  InterfaceMap_t create_interfaces(
      std::shared_ptr<CoreContext>) { /* <Get Interfaces from ServiceManager and
                                         load into InterfaceMap_t */
    InterfaceMap_t empty;
    return empty;
  }
  static const uint core_version{CORE_VERSION};
};

class MockCorePluginManager : virtual public PluginManagerBase {
public:
  MockCorePluginManager() {}
  ~MockCorePluginManager() {}
  MOCK_METHOD(void, load, (std::shared_ptr<CoreContext>), (override));
  MOCK_METHOD(void, unload, (), (override));
  MOCK_METHOD(void, load_plugin, (std::string), (override));
  MOCK_METHOD(void, _register_plugin,
              (std::type_index,
               (std::pair<std::string, std::shared_ptr<PluginBase>>)),
              (override));
  MOCK_METHOD(void, _unload, (std::type_index), (override));
  MOCK_METHOD(void, _reload, (std::type_index), (override));
  MOCK_METHOD(void, service_unload_notify, (std::type_index), (override));
  MOCK_METHOD(std::thread, service_reload_warn,
              (std::shared_ptr<std::mutex>, std::shared_ptr<std::type_index>),
              (override));
};