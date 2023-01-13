#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "events.h"
#include "logging.h"
#include "service_loader.h"
#include "servicelib.h"
#include "utils/lib_loader.h"
#include "utils/defines.h"

#include "service_test_mock.h"

class ServiceLoaderTest : public testing::Test {
protected:
  std::shared_ptr<ServiceTestServiceManager> service_manager =
      std::make_shared<ServiceTestServiceManager>();
  std::shared_ptr<ServiceTestEventServer> event_server =
      std::make_shared<ServiceTestEventServer>();
  std::shared_ptr<ServiceTestEventClient> event_client =
      std::make_shared<ServiceTestEventClient>();

  std::unique_ptr<LibLoader<ServiceLoader>> dut;
};

TEST_F(ServiceLoaderTest, setup) {
  std::shared_ptr<CoreContext> context = std::make_shared<CoreContext>(
      plog::get(), event_server, service_manager, TEST_LIB_DIR);

  EXPECT_CALL(*event_server, create_client())
      .WillRepeatedly(testing::Return(event_client));
  EXPECT_CALL(*event_client, subscribe(testing::_))
      .WillRepeatedly(testing::Return());

  EXPECT_CALL(*service_manager, register_service(testing::_, testing::_))
      .WillOnce(testing::Return());

  EXPECT_CALL(*event_client, receive())
      .WillRepeatedly(testing::Return(NULL_EVENT));

  dut = std::make_unique<LibLoader<ServiceLoader>>(
      std::dynamic_pointer_cast<Context>(context));
}
