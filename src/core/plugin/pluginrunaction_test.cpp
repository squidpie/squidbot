/*
pluginrunaction_test.cpp
Copyright (C) 2023  Squidpie
 */

#include <gtest/gtest.h>

#include "mock_core.h"
#include "utils/defines.h"

class PluginRunActionTest : public testing::Test {
protected:
  std::shared_ptr<MockCoreEventClient> client =
      std::make_shared<MockCoreEventClient>();
  std::shared_ptr<MockCorePluginFilter> filter =
      std::make_shared<MockCorePluginFilter>();
  std::shared_ptr<MockCorePluginAction> action =
      std::make_shared<MockCorePluginAction>();

  std::shared_ptr<PluginRunActionContext> context =
      std::make_shared<PluginRunActionContext>(client, filter, action);

  PluginRunAction dut = PluginRunAction(context);

  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(PluginRunActionTest, rx) {
  Event rx{1, EVENTS.TEST_EVENT_TYPE};
  EXPECT_CALL(*client, receive()).WillOnce(testing::Return(rx));
  EXPECT_CALL(*filter, is_trigger(testing::Eq(rx)))
      .WillOnce(testing::Return(true));
  EXPECT_CALL(*action, trigger_action(testing::Eq(rx)))
      .WillOnce(testing::Return());
  dut.run_action();
}

TEST_F(PluginRunActionTest, null) {
  EXPECT_CALL(*client, receive()).WillOnce(testing::Return(NULL_EVENT));
  dut.run_action();
}