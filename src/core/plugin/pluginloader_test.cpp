#include <gtest/gtest.h>

#include "mock_core.h"
#include "pluginloader.h"
#include "utils/defines.h"

class PluginLoaderTest : public testing::Test {
  protected:
    std::shared_ptr<MockCoreEventServer> server = std::make_shared<MockCoreEventServer>();
    std::shared_ptr<MockCoreServiceManager> service_manager = std::make_shared<MockCoreServiceManager>();
    std::shared_ptr<CoreContext> context;

    void SetUp() override {
      context = std::make_shared<CoreContext>(plog::get(), server, service_manager, TEST_LIB_DIR);
    }
};

TEST_F(PluginLoaderTest, setup) {
  LibLoader<PluginLoader> dut = LibLoader<PluginLoader>(context);
}