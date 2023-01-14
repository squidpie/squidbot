#include <gmock/gmock.h>

#include "core.h"
#include "event_client.h"
#include "plugin/plugin.h"

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
  MOCK_METHOD(void, subscribe, (EventType), (override));
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
  MOCK_METHOD(void, register_service,
              (std::type_index, std::shared_ptr<ServiceBase>), (override));
  MOCK_METHOD(std::shared_ptr<ServiceInterfaceBase>, _get_interface, (std::type_index), (override));
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
};

class MockCorePluginAction : virtual public PluginActionBase {
  public:
    MockCorePluginAction() {}
    MockCorePluginAction(InterfaceMap_t*, std::shared_ptr<PluginDataBase>) {}
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
    MockCorePlugin(std::shared_ptr<EventClientBase>) { /* <Register Event Subscriptions> */ }
    ~MockCorePlugin() {}
    InterfaceMap_t create_interfaces(std::shared_ptr<CoreContext>) { /* <Get Interfaces from ServiceManager and load into InterfaceMap_t */ InterfaceMap_t empty; return empty; }
};