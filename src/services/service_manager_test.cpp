#include <gtest/gtest.h>

#include "service.h"
#include "service_manager.h"

class ServiceManagerTest: public testing::Test {
protected:
  ServiceManager dut = ServiceManager("/home/squidpie/Repos/squidbot/targets/lib/test/");
  ServiceContainer_t mockcontainer = ServiceContainer("mockservice");

  void SetUp() override {
    ASSERT_NO_THROW(dut.load(&mockcontainer));
  }
};

TEST_F(ServiceManagerTest, ServiceManager_run) {
  ASSERT_NO_THROW(mockcontainer.run());
}

TEST_F(ServiceManagerTest, ServiceManager_unload) {
  ASSERT_NO_THROW(dut.unload(&mockcontainer));
  EXPECT_THROW(mockcontainer.run(), ServiceRuntimeError);
}