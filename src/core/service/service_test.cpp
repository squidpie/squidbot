#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "events.h"
#include "runner.h"
#include "service.h"
#include "service_manager.h"
#include "servicelib.h"
#include "utils/defines.h"

#include "service_test_mock.h"

class ServiceTest : public testing::Test {
protected:
  std::shared_ptr<ServiceTestEventServer> event_server =
      std::make_shared<ServiceTestEventServer>();
  std::shared_ptr<ServiceTestEventClient> event_client =
      std::make_shared<ServiceTestEventClient>();
  std::shared_ptr<ServiceManager> service_manager =
      std::make_shared<ServiceManager>();

  std::shared_ptr<ServiceTestRunActionContext> run_action_context;
  std::shared_ptr<ServiceTestRunAction> run_action;
  std::shared_ptr<Runner<ServiceTestRunAction>> runner;

  std::shared_ptr<ServiceTestServiceData> service_data;

  std::shared_ptr<Service<ServiceTestMockService>> dut;

  void SetUp() override {

    plog::init(plog::debug, "mock.log");
    std::shared_ptr<CoreContext> context = std::make_shared<CoreContext>(
        plog::get(), event_server, service_manager, TEST_LIB_DIR);

    run_action_context = std::make_shared<ServiceTestRunActionContext>(
        event_client, service_data);

    run_action = std::make_shared<ServiceTestRunAction>(run_action_context);

    runner = std::make_shared<Runner<ServiceTestRunAction>>(run_action_context);
    runner->inject(run_action);

    EXPECT_CALL(*run_action, run_action).WillRepeatedly(testing::Return());

    dut = std::make_shared<Service<ServiceTestMockService>>();
    dut->inject(runner, service_data);

    dut->start();
  }

  void TearDown() override { dut->stop(); }
};

TEST_F(ServiceTest, SetUp) { sleep(1); }