#include <gtest/gtest.h>

#include "service.h"
#include "service_manager.h"

class SquidbotTest: public testing::Test {
protected:

  const std::string TEST_LIB_DIR = "/home/squidpie/Repos/squidbot/targets/lib/test/";

  void SetUp() override {
    auto service_manager = ServiceManager(TEST_LIB_DIR);
    auto mock = ServiceContainer("mockservice");

    ASSERT_NO_THROW(service_manager.load(&mock));
    ASSERT_TRUE(mock.run());
  }
};

TEST_F(SquidbotTest, Squidbot_main) {}