#include <gtest/gtest.h>

#include "mock_core.h"

class RunnerTest : public testing::Test {
protected:
  std::shared_ptr<MockCoreRunActionContext> run_action_context =
      std::make_shared<MockCoreRunActionContext>();
  std::shared_ptr<MockCoreRunAction> mock_run_action =
      std::make_shared<MockCoreRunAction>(run_action_context);

  std::unique_ptr<Runner<MockCoreRunAction>> dut =
      std::make_unique<Runner<MockCoreRunAction>>();

  void SetUp() override {
    dut->inject(mock_run_action);
    EXPECT_CALL(*mock_run_action, run_action())
        .WillRepeatedly(testing::Return());
    dut->start();
  }

  void TearDown() override { dut->stop(); }
};

TEST_F(RunnerTest, SetUp) { sleep(1); }