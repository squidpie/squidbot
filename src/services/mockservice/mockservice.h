#pragma once

#include "framework.h"

class MockServiceData : virtual public ServiceDataBase {
public:
  ~MockServiceData() {}
};

class MockServiceRunActionContext : virtual public RunActionContextBase {
public:
  ~MockServiceRunActionContext() {}
  std::shared_ptr<EventClientBase> event_client;
  std::shared_ptr<MockServiceData> mock_data;
};

class MockServiceRunAction : virtual public RunActionBase {
public:
  MockServiceRunAction(std::shared_ptr<MockServiceRunActionContext> context) {
    event_client = context->event_client;
    mock_data = context->mock_data;
  }
  void run_action();

protected:
  std::shared_ptr<EventClientBase> event_client;
  std::shared_ptr<MockServiceData> mock_data;
};

typedef Service<MockServiceRunAction, MockServiceRunActionContext,
                MockServiceData>
    MockService_t;