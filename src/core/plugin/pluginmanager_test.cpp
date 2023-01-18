#include <gtest/gtest.h>

#include "mock_core.h"
#include "mockplugin/mockplugin.h"
#include "mockservice/mockservice.h"
#include "utils/defines.h"

class PluginManagerTest : public testing::Test {
protected:
  std::shared_ptr<MockCoreEventClient> client =
      std::make_shared<MockCoreEventClient>();
  std::shared_ptr<MockCoreEventServer> server =
      std::make_shared<MockCoreEventServer>();
  std::shared_ptr<MockCoreServiceManager> service_manager =
      std::make_shared<MockCoreServiceManager>();
  std::shared_ptr<CoreContext> context;

  std::shared_ptr<ServiceInterfaceBase> interface;

  std::shared_ptr<PluginManager> dut;
  std::shared_ptr<PluginMap_t> plugins = std::make_shared<PluginMap_t>();
  std::shared_ptr<std::vector<std::unique_ptr<std::thread>>> unload_threads =
      std::make_shared<std::vector<std::unique_ptr<std::thread>>>();

  void SetUp() override {
    dut = std::make_shared<PluginManager>();
    context = std::make_shared<CoreContext>(plog::get(), server,
                                            service_manager, dut, TEST_LIB_DIR);
    dut->inject(plugins, unload_threads);

    Event rx = {0, EVENTS.TEST_EVENT_TYPE};
    EXPECT_CALL(*client, receive()).WillRepeatedly(testing::Return(rx));

    EXPECT_CALL(*server, create_client()).WillOnce(testing::Return(client));
    EXPECT_CALL(*client, subscribe(testing::Eq(EVENTS.TEST_EVENT_TYPE)))
        .WillOnce(testing::Return());
    EXPECT_CALL(*service_manager, _get_interface(testing::Eq(
                                      std::type_index(typeid(MockService)))))
        .WillOnce(testing::Return(interface));
    dut->load(context);

    EXPECT_EQ(plugins->size(), 1);
    EXPECT_EQ(plugins->at(std::type_index(typeid(MockPlugin))).first,
              TEST_LIB_DIR + "/plugins/libmockplugin.so");
  }

  void TearDown() override { dut->unload(); }
};
TEST_F(PluginManagerTest, unload_load) {
  dut->unload_plugin<MockPlugin>();
  EXPECT_CALL(*server, create_client()).WillOnce(testing::Return(client));
  EXPECT_CALL(*client, subscribe(testing::Eq(EVENTS.TEST_EVENT_TYPE)))
      .WillOnce(testing::Return());
  EXPECT_CALL(*service_manager,
              _get_interface(testing::Eq(std::type_index(typeid(MockService)))))
      .WillOnce(testing::Return(interface));
  dut->load_plugin("libmockplugin.so");
  EXPECT_FALSE(plugins->empty());
}

TEST_F(PluginManagerTest, reload_plugin) {
  EXPECT_CALL(*server, create_client()).WillOnce(testing::Return(client));
  EXPECT_CALL(*client, subscribe(testing::Eq(EVENTS.TEST_EVENT_TYPE)))
      .WillOnce(testing::Return());
  EXPECT_CALL(*service_manager,
              _get_interface(testing::Eq(std::type_index(typeid(MockService)))))
      .WillOnce(testing::Return(interface));
  dut->reload_plugin<MockPlugin>();
  EXPECT_EQ(plugins->size(), 1);
  EXPECT_EQ(plugins->at(std::type_index(typeid(MockPlugin))).first,
            TEST_LIB_DIR + "/plugins/libmockplugin.so");
  EXPECT_EQ(unload_threads->size(), 1);
}

TEST_F(PluginManagerTest, service_unload_notify) {
  dut->service_unload_notify(std::type_index(typeid(MockService)));
  EXPECT_TRUE(plugins->empty());
}

TEST_F(PluginManagerTest, service_reload_warn) {
  EXPECT_CALL(*server, create_client()).WillOnce(testing::Return(client));
  EXPECT_CALL(*client, subscribe(testing::Eq(EVENTS.TEST_EVENT_TYPE)))
      .WillOnce(testing::Return());
  EXPECT_CALL(*service_manager,
              _get_interface(testing::Eq(std::type_index(typeid(MockService)))))
      .WillOnce(testing::Return(interface));

  auto lock = std::make_shared<std::mutex>();
  auto index = std::make_shared<std::type_index>(typeid(MockService));
  auto thread = dut->service_reload_warn(lock, index);
  thread.join();
}
