#include <gtest/gtest.h>

#include <thread>
#include <unistd.h>

#include "pluginmanager.h"
#include "service_manager.h"
#include "squidbot.h"

class SquidbotTest : public testing::Test {
protected:
  std::shared_ptr<EventServer> event_server = std::make_shared<EventServer>();
  std::shared_ptr<ServiceManager> service_manager =
      std::make_shared<ServiceManager>();
  std::shared_ptr<PluginManager> plugin_manager =
      std::make_shared<PluginManager>();
  std::shared_ptr<CoreContext> context;

  void SetUp() override {
    plog::init(plog::debug, "mock.log");
    context = std::make_shared<CoreContext>(plog::get(), event_server,
                                            service_manager, plugin_manager,
                                            TEST_LIB_DIR);

    service_manager->load(context);
    plugin_manager->load(context);
  }

  void TearDown() override {
    plugin_manager->unload();
    service_manager->unload();
  }
};

TEST_F(SquidbotTest, main) { sleep(5); }