#include <gtest/gtest.h>

#include "mock_core.h"
#include "plugin.h"
#include "utils/defines.h"

class PluginTest : public testing::Test {
  protected:
    std::shared_ptr<MockCoreRunner> runner = std::make_shared<MockCoreRunner>();

    Plugin<MockCorePlugin> dut;
    
    void SetUp() override {
      dut.inject(runner);
    }

    void TearDown() override {
      EXPECT_CALL(*runner, stop()).WillOnce(testing::Return());
    }
};

TEST_F(PluginTest, start_stop) {
  EXPECT_CALL(*runner, start()).WillOnce(testing::Return());
  dut.start();
  EXPECT_CALL(*runner, stop()).WillOnce(testing::Return());
  dut.stop();
}

TEST_F(PluginTest, context_construct) {
  std::shared_ptr<MockCoreEventClient> client = std::make_shared<MockCoreEventClient>();
  std::shared_ptr<MockCoreEventServer> server = std::make_shared<MockCoreEventServer>();
  std::shared_ptr<MockCoreServiceManager> service_manager = std::make_shared<MockCoreServiceManager>();
  std::shared_ptr<CoreContext> context = std::make_shared<CoreContext>(plog::get(), server, service_manager, TEST_LIB_DIR);

  EXPECT_CALL(*server, create_client()).WillOnce(testing::Return(client));
  EXPECT_CALL(*client, subscribe(testing::Eq(TEST_EVENT_TYPE))).WillOnce(testing::Return());
  Plugin<MockCorePlugin> construct_dut = Plugin<MockCorePlugin>(context);
}