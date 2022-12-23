#include <gtest/gtest.h>

#include <thread>
#include <unistd.h>

#include "squidbot.h"

class SquidbotTest: public testing::Test {
protected:

  const std::string TEST_LIB_DIR = "/home/squidpie/Repos/squidbot/targets/lib/test/";
  ServiceManager service_manager = ServiceManager(TEST_LIB_DIR);
  ServiceContainer_t mock = ServiceContainer("mockservice");

  void SetUp() override {
    plog::init(plog::debug, "mock.log");
  }

};

TEST_F(SquidbotTest, Squidbot_main) {
  ASSERT_NO_THROW(service_manager.load(&mock));
  ASSERT_TRUE(mock.run());
}