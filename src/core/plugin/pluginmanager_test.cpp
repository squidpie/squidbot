/*
pluginmanager_test.cpp
Copyright (C) 2023  Squidpie
 */

#include <gtest/gtest.h>

#include "mock_core.h"
#include "mockplugin/mockplugin.h"
#include "mockservice/mockservice.h"
#include "utils/defines.h"

class PluginManagerTest : public testing::Test {
protected:
  std::shared_ptr<MockCoreEventClient> client{
      std::make_shared<MockCoreEventClient>()};
  std::shared_ptr<MockCoreEventServer> server{
      std::make_shared<MockCoreEventServer>()};
  std::shared_ptr<MockCoreServiceManager> service_manager{
      std::make_shared<MockCoreServiceManager>()};
  std::shared_ptr<CoreContext> context;

  std::shared_ptr<ServiceInterfaceBase> interface;

  std::shared_ptr<PluginManager> dut{std::make_shared<PluginManager>()};
  std::shared_ptr<PluginMap_t> plugins{std::make_shared<PluginMap_t>()};
  std::shared_ptr<std::vector<std::unique_ptr<std::thread>>> unload_threads =
      std::make_shared<std::vector<std::unique_ptr<std::thread>>>();

  void SetUp() override {
    context = std::make_shared<CoreContext>(plog::get(), server,
                                            service_manager, dut, TEST_LIB_DIR);
    dut->inject(plugins, unload_threads);

    Event rx{0, EVENTS.TEST_EVENT_TYPE};
    EXPECT_CALL(*client, receive()).WillRepeatedly(testing::Return(rx));

    EXPECT_CALL(*server, create_client()).WillOnce(testing::Return(client));
    EXPECT_CALL(*client, subscribe(testing::Eq(EVENTS.TEST_EVENT_TYPE)))
        .WillOnce(testing::Return());
    EXPECT_CALL(*service_manager, _get_interface(testing::Eq(
                                      std::type_index(typeid(MockService)))))
        .WillOnce(testing::Return(interface));
    dut->load(context);

    std::string lib_path = TEST_LIB_DIR;
    lib_path += "/plugins/libmockplugin.so";
    EXPECT_EQ(plugins->size(), 1);
    EXPECT_EQ(plugins->at(std::type_index(typeid(MockPlugin))).first, lib_path);
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

  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  EXPECT_EQ(unload_threads->size(), 1);
  unload_threads->at(0)->join();
  unload_threads->pop_back();

  EXPECT_EQ(plugins->size(), 1);
  EXPECT_TRUE(plugins->find(std::type_index(typeid(MockPlugin))) !=
              plugins->end());

  std::string lib_path = TEST_LIB_DIR;
  lib_path += "/plugins/libmockplugin.so";
  EXPECT_EQ(plugins->at(std::type_index(typeid(MockPlugin))).first, lib_path);
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

  auto lock{std::make_shared<std::mutex>()};
  auto index{std::make_shared<std::type_index>(typeid(MockService))};
  auto thread{dut->service_reload_warn(lock, index)};
  thread.join();
}

TEST_F(PluginManagerTest, register_plugin) {
  auto runner{std::make_shared<Runner<MockCoreRunAction>>()};
  InterfaceMap_t interfaces;
  auto mockcoreplugin{std::make_shared<Plugin<MockCorePlugin>>()};
  mockcoreplugin->inject(runner, interfaces);

  dut->register_plugin<MockCorePlugin>("mock_path", mockcoreplugin);
  EXPECT_EQ(plugins->size(), 2);
}