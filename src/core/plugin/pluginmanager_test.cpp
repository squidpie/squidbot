#include <gtest/gtest.h>

#include "mock_core.h"
#include "mockservice/mockservice.h"
#include "utils/defines.h"
#include "pluginmanager.h"

class PluginManagerTest : public testing::Test {
protected:
  std::shared_ptr<MockCoreEventClient> client = std::make_shared<MockCoreEventClient>();
  std::shared_ptr<MockCoreEventServer> server = std::make_shared<MockCoreEventServer>();
  std::shared_ptr<MockCoreServiceManager> service_manager = std::make_shared<MockCoreServiceManager>();
  std::shared_ptr<CoreContext> context;

  std::shared_ptr<ServiceInterfaceBase> interface;

  std::shared_ptr<PluginManager> dut;

  void SetUp() override {
    dut = std::make_shared<PluginManager>();
    context = std::make_shared<CoreContext>(plog::get(), server, service_manager, dut, TEST_LIB_DIR);
  }

  void TearDown() override { dut->unload(); }
};

TEST_F(PluginManagerTest, setup) {
  Event rx = {1, TEST_EVENT_TYPE};
  EXPECT_CALL(*server, create_client()).WillOnce(testing::Return(client));
  EXPECT_CALL(*client, subscribe(testing::Eq(TEST_EVENT_TYPE))).WillOnce(testing::Return());
  EXPECT_CALL(*service_manager, _get_interface(testing::Eq(std::type_index(typeid(MockService))))).WillOnce(testing::Return(interface));
  EXPECT_CALL(*client, receive()).WillRepeatedly(testing::Return(rx));
  dut->load(context);
  auto dump = dut->dump();
  EXPECT_TRUE(dump.size() > 0);
}