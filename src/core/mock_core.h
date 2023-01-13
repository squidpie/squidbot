#include <gmock/gmock.h>

#include "core.h"

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