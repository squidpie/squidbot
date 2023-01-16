#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "logging.h"
#include "mock_core.h"
#include "service_loader.h"
#include "utils/defines.h"
#include "utils/lib_loader.h"

class ServiceLoaderTest : public testing::Test {
protected:
  std::shared_ptr<MockCoreServiceManager> service_manager =
      std::make_shared<MockCoreServiceManager>();
  std::shared_ptr<MockCorePluginManager> plugin_manager =
      std::make_shared<MockCorePluginManager>();
  std::shared_ptr<MockCoreEventServer> event_server =
      std::make_shared<MockCoreEventServer>();
  std::shared_ptr<MockCoreEventClient> event_client =
      std::make_shared<MockCoreEventClient>();

  std::unique_ptr<LibLoader<ServiceLoader>> dut;
};

TEST_F(ServiceLoaderTest, setup) {
  std::shared_ptr<CoreContext> context = std::make_shared<CoreContext>(
      plog::get(), event_server, service_manager, plugin_manager, TEST_LIB_DIR);

  EXPECT_CALL(*event_server, create_client())
      .WillRepeatedly(testing::Return(event_client));
  EXPECT_CALL(*event_client, subscribe(testing::_))
      .WillRepeatedly(testing::Return());

  EXPECT_CALL(*service_manager, _register_service(testing::_, testing::_))
      .WillOnce(testing::Return());

  EXPECT_CALL(*event_client, receive())
      .WillRepeatedly(testing::Return(NULL_EVENT));

  dut = std::make_unique<LibLoader<ServiceLoader>>(
      std::dynamic_pointer_cast<Context>(context));
}
