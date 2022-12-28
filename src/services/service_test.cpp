#include "gmock/gmock.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "events.h"
#include "service.h"

const std::string TEST_LIB_PATH = "/home/squidpie/Repos/squidbot/targets/lib/test";

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

class ServiceTest: public testing::Test {
  protected:
    ServiceContainer_t * container_dut;
    ServiceContainer_t * error_dut;

    MockEventServer mock_event_server;
    MockEventClient mock_event_client;

    void SetUp() {
      plog::init(plog::debug, "mock.log");
      Context_t context = {plog::get(), &mock_event_server};

      ON_CALL(mock_event_server, create_client())
          .WillByDefault(testing::Return(&mock_event_client));

      EXPECT_CALL(mock_event_server, create_client);
      container_dut = new ServiceContainer("mockservice", &context);
      error_dut = new ServiceContainer("no_lib_found_error", &context);

      container_dut->gen_lib_path(TEST_LIB_PATH);
      ASSERT_NO_THROW(container_dut->init());
      ASSERT_NO_THROW(container_dut->create());
    }
};

TEST_F(ServiceTest, ServiceContainer_create) {}

TEST_F(ServiceTest, ServiceContainer_run) {
  ASSERT_NO_THROW(container_dut->run());
}

TEST_F(ServiceTest, ServiceContainer_destroy) {
  ASSERT_NO_THROW(container_dut->destroy());
  ASSERT_THROW(container_dut->run(), ServiceRuntimeError);
}

TEST_F(ServiceTest, ServiceContainer_throw) {
  error_dut->gen_lib_path(TEST_LIB_PATH);
  ASSERT_THROW(error_dut->init(), ServiceLibError);
}