#pragma once

#include "framework.h"
#include "service.h"

class MockServiceData : virtual public ServiceDataBase {
public:
  ~MockServiceData() {}
};

class MockServicePluginInterface : virtual public ServiceInterfaceBase {
public:
  MockServicePluginInterface() {}
  MockServicePluginInterface(std::shared_ptr<MockServiceData> data) : data(data) {  }
  ~MockServicePluginInterface() {}
  void test();
  protected:
    std::shared_ptr<MockServiceData> data;
};

class MockServiceExternalInterface : virtual public ServiceInterfaceBase {
public:
  ~MockServiceExternalInterface() {}
};

class MockServiceRunActionContext : virtual public RunActionContextBase {
public:
  MockServiceRunActionContext(std::shared_ptr<EventClientBase> event_client,
                              std::shared_ptr<MockServiceData> mock_data)
      : event_client(event_client), mock_data(mock_data) {}
  ~MockServiceRunActionContext() {}
  std::shared_ptr<EventClientBase> event_client;
  std::shared_ptr<MockServiceData> mock_data;
};

class MockServiceRunAction : virtual public RunActionBase {
public:
  typedef MockServiceRunActionContext context_t;
  MockServiceRunAction(std::shared_ptr<MockServiceRunActionContext> context) {
    event_client = context->event_client;
    mock_data = context->mock_data;
  }
  void run_action();

protected:
  std::shared_ptr<EventClientBase> event_client;
  std::shared_ptr<MockServiceData> mock_data;
};

class MockService {
public:
  typedef MockServiceRunAction run_action_t;
  typedef MockServicePluginInterface plugin_interface_t;
  typedef MockServiceExternalInterface external_interface_t;
  typedef MockServiceData data_t;
};

typedef Service<MockService> MockService_t;