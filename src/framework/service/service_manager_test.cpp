#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

#include "logging.h"
#include "mockservice/mockservice.h"
#include "service.h"
#include "service_manager.h"
#include "servicelib.h"

#include "service_test_mock.h"

/*
what does a service_manager do?
    loads the .so file create/destroy functions into memory.
    manages the creation and destruction of service memory.
    starts/stops service
    hands out interfaces to services
 */

class ServiceManagerTest : public testing::Test {
  protected:
    std::string test_lib_dir = "targets/lib/test";
    std::shared_ptr<ServiceTestEventServer> event_server = std::make_shared<ServiceTestEventServer>();
    std::shared_ptr<ServiceTestEventClient> event_client = std::make_shared<ServiceTestEventClient>();
    std::shared_ptr<ServiceManager> dut = std::make_shared<ServiceManager>();
    std::shared_ptr<ServiceContext> context;



    void SetUp() override {
      context = std::make_shared<ServiceContext>(plog::get(), event_server, dut, test_lib_dir);

      EXPECT_CALL(*event_server, create_client()).Times(2).WillRepeatedly(testing::Return(event_client));
      EXPECT_CALL(*event_client, subscribe(testing::_)).Times(2);
      EXPECT_CALL(*event_client, receive()).WillRepeatedly(testing::Return(NULL_EVENT));
      dut->load(context);

      auto mock_plugin_interface = dut->get_interface<MockService>();
      EXPECT_TRUE(mock_plugin_interface != NULL);
      mock_plugin_interface->test();
    }
    void TearDown() override {}
};

TEST_F(ServiceManagerTest, SetUp) {

}