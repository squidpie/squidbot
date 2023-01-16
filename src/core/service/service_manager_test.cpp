#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "logging.h"
#include "mockservice/mockservice.h"
#include "service.h"
#include "service_manager.h"
#include "servicelib.h"
#include "utils/defines.h"

#include "mock_core.h"

class ServiceManagerTest : public testing::Test {
protected:
  std::shared_ptr<MockCoreEventServer> event_server =
      std::make_shared<MockCoreEventServer>();
  std::shared_ptr<MockCoreEventClient> event_client =
      std::make_shared<MockCoreEventClient>();
  std::shared_ptr<MockCorePluginManager> plugin_manager =
      std::make_shared<MockCorePluginManager>();
  std::shared_ptr<CoreContext> context;

  std::shared_ptr<ServiceManager> dut = std::make_shared<ServiceManager>();
  std::shared_ptr<ServiceMap_t> services = std::make_shared<ServiceMap_t>();

  void SetUp() override {
    context = std::make_shared<CoreContext>(plog::get(), event_server, dut,
                                            plugin_manager, TEST_LIB_DIR);

    dut->inject(services);

    EXPECT_CALL(*event_client, receive())
        .WillRepeatedly(testing::Return(NULL_EVENT));

    EXPECT_CALL(*event_server, create_client())
        .WillOnce(testing::Return(event_client));
    EXPECT_CALL(*event_client, subscribe(testing::_));
    dut->load(context);
    EXPECT_FALSE(services->empty());
  }
  void TearDown() override { dut->unload(); }
};

TEST_F(ServiceManagerTest, mockservice_interface) {
  auto mock_plugin_interface = dut->get_interface<MockService>();
  EXPECT_TRUE(mock_plugin_interface != nullptr);
  mock_plugin_interface->test();
}

TEST_F(ServiceManagerTest, null_interface) {
  EXPECT_TRUE(dut->get_interface<MockCoreMockService>() == nullptr);
}

TEST_F(ServiceManagerTest, service_unload_load) {
  EXPECT_CALL(*plugin_manager, service_unload_notify(testing::Eq(
                                   std::type_index(typeid(MockService)))));
  dut->unload_service<MockService>();
  EXPECT_TRUE(services->empty());

  EXPECT_CALL(*event_server, create_client())
      .WillOnce(testing::Return(event_client));
  EXPECT_CALL(*event_client, subscribe(testing::_));
  dut->load_service("libmockservice.so");
  EXPECT_FALSE(services->empty());
}

TEST_F(ServiceManagerTest, service_reload) {
  EXPECT_CALL(*plugin_manager,
              service_reload_warn(testing::_, testing::_))
      .WillOnce(testing::Return(std::thread([] {})));
  EXPECT_CALL(*event_server, create_client())
      .WillOnce(testing::Return(event_client));
  EXPECT_CALL(*event_client, subscribe(testing::_));
  dut->reload_service<MockService>();
  EXPECT_FALSE(services->empty());
}