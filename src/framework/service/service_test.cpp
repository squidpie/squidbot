#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

#include "events.h"
#include "runner.h"
#include "service.h"
#include "service_manager.h"
#include "servicelib.h"

const std::string TEST_LIB_PATH =
    "/home/squidpie/Repos/squidbot/targets/lib/test";

class ServiceTestMockEventClient : public EventClientBase {
public:
  MOCK_METHOD(void, subscribe, (EventType));
  MOCK_METHOD(void, send, (Event));
  MOCK_METHOD(const Event, receive, ());
};

class ServiceTestMockEventServer : public EventServerBase {
public:
  MOCK_METHOD(std::shared_ptr<EventClientBase>, create_client, ());
};

class ServiceTestServiceData : virtual public ServiceDataBase {
public:
  ~ServiceTestServiceData() {}
};


class ServiceTestPluginInterface : virtual public ServiceInterfaceBase {
public:
  ServiceTestPluginInterface(std::shared_ptr<ServiceTestServiceData> data) : data(data) {}
  ~ServiceTestPluginInterface() {}
  protected:
    std::shared_ptr<ServiceTestServiceData> data;

};

class ServiceTestExternalInterface : virtual public ServiceInterfaceBase {
  ~ServiceTestExternalInterface() {}
};

class ServiceTestRunActionContext : virtual public RunActionContextBase {
public:
  ServiceTestRunActionContext(std::shared_ptr<EventClientBase> event_client,
                              std::shared_ptr<ServiceTestServiceData> data)
      : event_client(event_client), data(data) {}
  ~ServiceTestRunActionContext() {}
  std::shared_ptr<EventClientBase> event_client;
  std::shared_ptr<ServiceTestServiceData> data;
};

class ServiceTestRunAction : virtual public RunActionBase {
public:
  typedef ServiceTestRunActionContext context_t;
  ServiceTestRunAction(std::shared_ptr<ServiceTestRunActionContext> _context) {
    event_client = _context->event_client;
  }
  MOCK_METHOD(void, run_action, (), (override));
  std::shared_ptr<EventClientBase> event_client;
};

class ServiceTestMockService {
public:
  typedef ServiceTestRunAction run_action_t;
  typedef ServiceTestServiceData data_t;
  typedef ServiceTestPluginInterface plugin_interface_t;
  typedef ServiceTestExternalInterface external_interface_t;
};

class ServiceTest : public testing::Test {
protected:
  std::shared_ptr<ServiceTestMockEventServer> mock_event_server = std::make_shared<ServiceTestMockEventServer>();
  std::shared_ptr<ServiceTestMockEventClient> mock_event_client =
      std::make_shared<ServiceTestMockEventClient>();

  std::shared_ptr<ServiceTestRunActionContext> mock_run_action_context;
  std::shared_ptr<ServiceTestRunAction> mock_run_action;
  std::shared_ptr<ServiceTestServiceData> mock_service_data;
  std::shared_ptr<Runner<ServiceTestRunAction>> mock_runner;

  std::shared_ptr<Service<ServiceTestMockService>> dut;

  testing::Sequence se;

  std::shared_ptr<ServiceManager> service_manager = std::make_shared<ServiceManager>();

  void SetUp() override {

    plog::init(plog::debug, "mock.log");
    std::shared_ptr<ServiceContext> context = std::make_shared<ServiceContext>(plog::get(), mock_event_server, service_manager, TEST_LIB_PATH);

    mock_run_action_context = std::make_shared<ServiceTestRunActionContext>(
        mock_event_client, mock_service_data);
    mock_run_action_context->event_client = mock_event_client;

    mock_run_action =
        std::make_shared<ServiceTestRunAction>(mock_run_action_context);

    mock_runner =
        std::make_shared<Runner<ServiceTestRunAction>>(mock_run_action_context);
    mock_runner->inject(mock_run_action);

    EXPECT_CALL(*mock_event_server, create_client)
        .InSequence(se)
        .WillRepeatedly(
            testing::Invoke([this]() { return mock_event_client; }));

    EXPECT_CALL(*mock_event_client,
                subscribe(testing::Eq<EventType>(TEST_EVENT_TYPE)))
        .InSequence(se);
    EXPECT_CALL(*mock_run_action, run_action).WillRepeatedly(testing::Return());

    dut = std::make_shared<Service<ServiceTestMockService>>(context);

    dut->inject(mock_runner, mock_service_data);
    dut->start();
  }

  void TearDown() override { dut->stop(); }
};

TEST_F(ServiceTest, SetUp) {
  sleep(1);
  //EXPECT_TRUE(      testing::Mock::VerifyAndClearExpectations(mock_event_client.get()));
}