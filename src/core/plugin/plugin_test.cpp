#include <gtest/gtest.h>

#include "event_server.h"
#include "mock_core.h"
#include "utils/defines.h"

class PluginTest : public testing::Test {
protected:
  std::shared_ptr<MockCoreRunner> runner = std::make_shared<MockCoreRunner>();
  InterfaceMap_t interfaces;

  Plugin<MockCorePlugin> dut;

  void SetUp() override {
    interfaces.insert({std::type_index(typeid(MockCoreMockService)),
                       std::make_shared<MockCorePluginInterface>(
                           std::make_shared<MockCoreServiceData>())});
    dut.inject(runner, interfaces);
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
  std::shared_ptr<MockCoreEventClient> client =
      std::make_shared<MockCoreEventClient>();
  std::shared_ptr<MockCoreEventServer> server =
      std::make_shared<MockCoreEventServer>();
  std::shared_ptr<MockCoreServiceManager> service_manager =
      std::make_shared<MockCoreServiceManager>();
  std::shared_ptr<MockCorePluginManager> plugin_manager =
      std::make_shared<MockCorePluginManager>();
  std::shared_ptr<CoreContext> context = std::make_shared<CoreContext>(
      plog::get(), server, service_manager, plugin_manager, TEST_LIB_DIR);

  EXPECT_CALL(*server, create_client()).WillOnce(testing::Return(client));
  EXPECT_CALL(*client, subscribe(testing::Eq(EVENTS.TEST_EVENT_TYPE)))
      .WillOnce(testing::Return());
  Plugin<MockCorePlugin> construct_dut = Plugin<MockCorePlugin>(context);
}

TEST_F(PluginTest, dependency) {
  EXPECT_TRUE(dut.is_dependent(std::type_index(typeid(MockCoreMockService))));
}

TEST_F(PluginTest, core_version) {
  EXPECT_TRUE(dut.core_version() == CORE_VERSION);
}