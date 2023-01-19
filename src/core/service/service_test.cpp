#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mock_core.h"
#include "utils/defines.h"

class ServiceTest : public testing::Test {
protected:
  std::shared_ptr<MockCoreEventServer> event_server =
      std::make_shared<MockCoreEventServer>();
  std::shared_ptr<MockCoreEventClient> event_client =
      std::make_shared<MockCoreEventClient>();
  std::shared_ptr<ServiceManager> service_manager =
      std::make_shared<ServiceManager>();
  std::shared_ptr<MockCorePluginManager> plugin_manager =
      std::make_shared<MockCorePluginManager>();

  std::shared_ptr<MockCoreRunActionContext> run_action_context =
      std::make_shared<MockCoreRunActionContext>();
  std::shared_ptr<MockCoreRunAction> run_action =
      std::make_shared<MockCoreRunAction>(run_action_context);
  std::shared_ptr<Runner<MockCoreRunAction>> runner =
      std::make_shared<Runner<MockCoreRunAction>>(run_action_context);

  std::shared_ptr<MockCoreServiceData> service_data;

  std::shared_ptr<Service<MockCoreMockService>> dut;

  void SetUp() override {
    std::shared_ptr<CoreContext> context = std::make_shared<CoreContext>(
        plog::get(), event_server, service_manager, plugin_manager,
        TEST_LIB_DIR);

    runner->inject(run_action);

    dut = std::make_shared<Service<MockCoreMockService>>();
    dut->inject(runner, service_data);

    EXPECT_CALL(*run_action, run_action).WillRepeatedly(testing::Return());
    dut->start();
  }

  void TearDown() override { dut->stop(); }
};

TEST_F(ServiceTest, start_stop) {}

TEST_F(ServiceTest, core_version) {
  EXPECT_TRUE(dut->core_version() == CORE_VERSION);
}