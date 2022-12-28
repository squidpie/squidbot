#include <gtest/gtest.h>

#include "events.h"
#include "service.h"
#include "service_manager.h"
#include "utils.h"

class ServiceManagerTest: public testing::Test {
protected:
  ServiceManager dut = ServiceManager("/home/squidpie/Repos/squidbot/targets/lib/test/");
  ServiceContainer_t * mockcontainer;
  EventServer mock_buffer_server;

  void SetUp() override {
    plog::init(plog::debug, "mock.log");
    Context_t context = Context_t {plog::get(), &mock_buffer_server};
    mockcontainer = new ServiceContainer("mockservice", &context);
    ASSERT_NO_THROW(dut.load(mockcontainer));
  }
};

TEST_F(ServiceManagerTest, ServiceManager_run) {
  ASSERT_NO_THROW(mockcontainer->run());
}

TEST_F(ServiceManagerTest, ServiceManager_unload) {
  ASSERT_NO_THROW(dut.unload(mockcontainer));
  EXPECT_THROW(mockcontainer->run(), ServiceRuntimeError);
}