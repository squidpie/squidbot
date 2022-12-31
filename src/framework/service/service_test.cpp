#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

#include "events.h"
#include "runner.h"
#include "service.h"

const std::string TEST_LIB_PATH =
    "/home/squidpie/Repos/squidbot/targets/lib/test";

const int MOCK_EVENT_CLIENT_ID_TX = 1;
const int MOCK_EVENT_CLIENT_ID_RX = 2;

class ServiceTestEventClient : public EventClientBase {
public:
  MOCK_METHOD(void, subscribe, (EventType));
  MOCK_METHOD(void, send, (Event));
  MOCK_METHOD(const Event, receive, ());
};

class ServiceTestEventServer : public EventServerBase {
public:
  MOCK_METHOD(std::shared_ptr<EventClientBase>, create_client, ());
};

class ServiceTestRunActionContext : virtual public RunActionContextBase {
  public:
    ~ServiceTestRunActionContext() {}
    //MOCK_METHOD(void, _init, ());
    std::shared_ptr<EventClientBase> event_client;
};

class ServiceTestRunAction : virtual public RunActionBase {
  public:
    ServiceTestRunAction(std::shared_ptr<ServiceTestRunActionContext> _context) {
      event_client = _context->event_client;
    }
    MOCK_METHOD(void, run_action, (), (override));
    std::shared_ptr<EventClientBase> event_client;
};

class MockServiceData : virtual public ServiceDataBase {
  public:
    MOCK_METHOD(void, _init, ());
};


class ServiceTest : public testing::Test {
protected:
  ServiceTestEventServer mock_event_server;
  std::shared_ptr<ServiceTestEventClient> mock_event_client = std::make_shared<ServiceTestEventClient>();

  std::shared_ptr<ServiceTestRunActionContext> mock_run_action_context;
  std::shared_ptr<ServiceTestRunAction> mock_run_action;
  std::shared_ptr<MockServiceData> mock_service_data;
  std::shared_ptr<Runner<ServiceTestRunAction, ServiceTestRunActionContext>> mock_runner;

  std::unique_ptr<Service<ServiceTestRunAction, ServiceTestRunActionContext, MockServiceData>> dut;

  testing::Sequence se;

  void SetUp() override {

    plog::init(plog::debug, "mock.log");
    ServiceContext context = {
        plog::get(), dynamic_cast<EventServerBase *>(&mock_event_server)};

    mock_run_action_context = std::make_shared<ServiceTestRunActionContext>();
    mock_run_action_context->event_client = mock_event_client;

    mock_run_action = std::make_shared<ServiceTestRunAction>(mock_run_action_context);

    mock_runner = std::make_shared<Runner<ServiceTestRunAction, ServiceTestRunActionContext>>(mock_run_action_context);
    mock_runner->inject(mock_run_action);

   /* ON_CALL(mock_event_server, create_client())
        .WillByDefault(testing::Invoke([this]() {
          std::cerr<<"MEOW";
          EXPECT_TRUE(mock_event_client != NULL);
          return mock_event_client;
        }));
*/
    EXPECT_CALL(mock_event_server, create_client)
        .InSequence(se)
        .WillRepeatedly(testing::Invoke([this]() {
          return mock_event_client;
        }));

    EXPECT_CALL(*mock_event_client,
                subscribe(testing::Eq<EventType>(TEST_EVENT_TYPE)))
        .InSequence(se);

    dut.reset(new Service<ServiceTestRunAction, ServiceTestRunActionContext, MockServiceData>(context));
    Service<ServiceTestRunAction, ServiceTestRunActionContext, MockServiceData>meow = *dut;
    dut->inject(mock_runner, mock_run_action_context, mock_service_data);

    EXPECT_CALL(*mock_run_action, run_action).WillRepeatedly(testing::Return());
    dut->start();
  }

  void TearDown() override {
    dut->stop();
  }
};

TEST_F(ServiceTest, SetUp) {
  //Event e_rx =
  //    Event{.source = MOCK_EVENT_CLIENT_ID_RX, .type = TEST_EVENT_TYPE};
  //Event e_tx =
  //    Event{.source = MOCK_EVENT_CLIENT_ID_TX, .type = TEST_EVENT_TYPE};
  //EXPECT_CALL(mock_event_client, receive)
  //    .InSequence(se)
  //    .WillOnce(testing::Return(e_rx));
  //EXPECT_CALL(mock_event_client, send(testing::Eq<Event>(e_rx)))
  //    .InSequence(se)
  //    .WillOnce(testing::Return());
  //EXPECT_CALL(mock_event_client, receive)
  //    .InSequence(se)
  //    .WillRepeatedly(testing::Return(NULL_EVENT));

  sleep(1);
  EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(mock_event_client.get()));
}