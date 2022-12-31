#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "events.h"
#include "service.h"
#include "service_manager.h"
#include "utils.h"

class MockEventClient : public EventClientBase {
public:
  MOCK_METHOD(void, subscribe, (EventType));
  MOCK_METHOD(void, send, (Event));
  MOCK_METHOD(const Event, receive, ());
};

class MockEventServer : public EventServerBase {
public:
  MOCK_METHOD(EventClientBase*, create_client, ());
};

class MockServiceContainer : public ServiceContainerBase {
  public:
  MOCK_METHOD(void, gen_lib_path, (std::string));
  MOCK_METHOD(void, init, ());
  MOCK_METHOD(void, create, ());
  MOCK_METHOD(void, destroy, ());
  MOCK_METHOD(void, run, ());
};

class ServiceManagerTest: public testing::Test {
protected:
  ServiceManager dut = ServiceManager("/home/squidpie/Repos/squidbot/targets/lib/test/");
  MockServiceContainer* mock_container;
  MockEventServer mock_event_server;

  void SetUp() override {
    plog::init(plog::debug, "mock.log");
    Context_t context = Context_t {plog::get(), &mock_event_server};

    mock_container = new MockServiceContainer();
    EXPECT_CALL(*mock_container, gen_lib_path(testing::_));
    EXPECT_CALL(*mock_container, init());
    EXPECT_CALL(*mock_container, create());
    ASSERT_NO_THROW(dut.load(mock_container));
  }

  void TearDown() override {
    delete mock_container;
  }
};

TEST_F(ServiceManagerTest, ServiceManager_run) {
  EXPECT_CALL(*mock_container, run());
  ASSERT_NO_THROW(mock_container->run());
}

TEST_F(ServiceManagerTest, DISABLED_ServiceManager_unload) {
  ASSERT_NO_THROW(dut.unload(mock_container));
  EXPECT_THROW(mock_container->run(), ServiceRuntimeError);
}