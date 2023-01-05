#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "runner.h"

class RunnerTestRunActionContext : virtual public RunActionContextBase {
  public:
    ~RunnerTestRunActionContext() {}
};

class RunnerTestRunAction : virtual public RunActionBase {
  public:
    typedef RunnerTestRunActionContext context_t;
    RunnerTestRunAction(std::shared_ptr<RunnerTestRunActionContext> context) {}
    MOCK_METHOD(void , run_action, (), (override));
};

class RunnerTest : public testing::Test {
  protected:
    std::shared_ptr<RunnerTestRunActionContext> run_action_context = std::make_shared<RunnerTestRunActionContext>();
    
    std::shared_ptr<RunnerTestRunAction> mock_run_action = std::make_shared<RunnerTestRunAction>(run_action_context);
    std::unique_ptr<Runner<RunnerTestRunAction>> dut = std::make_unique<Runner<RunnerTestRunAction>>(run_action_context);

    void SetUp() override {
      dut->inject(mock_run_action);
      EXPECT_CALL(*mock_run_action, run_action()).WillRepeatedly(testing::Return());
      dut->start();
    }

    void TearDown() override {
      dut->stop();
    }
};

TEST_F(RunnerTest, SetUp) {
  sleep(1);
}