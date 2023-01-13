#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "logging.h"
#include "mockservice/mockservice.h"
#include "service.h"
#include "service_manager.h"
#include "servicelib.h"
#include "utils/defines.h"

#include "service_test_mock.h"

class ServiceManagerTest : public testing::Test {
protected:
  std::shared_ptr<ServiceTestEventServer> event_server =
      std::make_shared<ServiceTestEventServer>();
  std::shared_ptr<ServiceTestEventClient> event_client =
      std::make_shared<ServiceTestEventClient>();
  std::shared_ptr<ServiceContext> context;

  std::shared_ptr<ServiceManager> dut = std::make_shared<ServiceManager>();


  void SetUp() override {
    context = std::make_shared<ServiceContext>(plog::get(), event_server, dut,
                                               TEST_LIB_DIR);

    EXPECT_CALL(*event_server, create_client())
        .WillOnce(testing::Return(event_client));
    EXPECT_CALL(*event_client, subscribe(testing::_));
    EXPECT_CALL(*event_client, receive())
        .WillRepeatedly(testing::Return(NULL_EVENT));
    dut->load(context);
  }
  void TearDown() override {}
};

TEST_F(ServiceManagerTest, mockservice_interface) {
  auto mock_plugin_interface = dut->get_interface<MockService>();
  EXPECT_TRUE(mock_plugin_interface != nullptr);
  mock_plugin_interface->test();
}

TEST_F(ServiceManagerTest, null_interface) {
  EXPECT_TRUE(dut->get_interface<ServiceTestMockService>() == nullptr);
}