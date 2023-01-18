#include <gtest/gtest.h>

#include "mock_core.h"
#include "mockservice/mockservice.h"
#include "pluginloader.h"
#include "utils/defines.h"

class PluginLoaderTest : public testing::Test {
protected:
  std::shared_ptr<MockCoreEventClient> client =
      std::make_shared<MockCoreEventClient>();
  std::shared_ptr<MockCoreEventServer> server =
      std::make_shared<MockCoreEventServer>();
  std::shared_ptr<MockCoreServiceManager> service_manager =
      std::make_shared<MockCoreServiceManager>();
  std::shared_ptr<MockCorePluginManager> plugin_manager =
      std::make_shared<MockCorePluginManager>();
  std::shared_ptr<CoreContext> context;

  std::shared_ptr<ServiceInterfaceBase> interface;

  void SetUp() override {
    context = std::make_shared<CoreContext>(
        plog::get(), server, service_manager, plugin_manager, TEST_LIB_DIR);
  }

  void TearDown() override {}
};

TEST_F(PluginLoaderTest, setup) {
  EXPECT_CALL(*client, receive()).WillRepeatedly(testing::Return(NULL_EVENT));
  EXPECT_CALL(*server, create_client()).WillOnce(testing::Return(client));
  EXPECT_CALL(*client, subscribe(testing::Eq(EVENTS.TEST_EVENT_TYPE)))
      .WillOnce(testing::Return());
  EXPECT_CALL(*service_manager,
              _get_interface(testing::Eq(std::type_index(typeid(MockService)))))
      .WillOnce(testing::Return(interface));
  EXPECT_CALL(*plugin_manager, _register_plugin(testing::_, testing::_))
      .WillOnce(testing::Return());
  LibLoader<PluginLoader> dut = LibLoader<PluginLoader>(context);
}