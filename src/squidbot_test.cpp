#include <gtest/gtest.h>

#include "corelib.h"
#include "events.h"
#include "logging.h"
#include "mockservice/mockservice.h"
#include "utils/defines.h"

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

    event_server->start();
    service_manager->load(context);
    plugin_manager->load(context);
  }

  void TearDown() override {
    event_server->stop();
    plugin_manager->unload();
    service_manager->unload();
  }
};

TEST_F(SquidbotTest, reload_service) {
  service_manager->reload_service<MockService>();
  std::this_thread::sleep_for(std::chrono::nanoseconds(1));
}