#include <gtest/gtest.h>

#include <thread>
#include <unistd.h>

#include "service_manager.h"
#include "squidbot.h"

class SquidbotTest: public testing::Test {
protected:
  std::shared_ptr<EventServer> event_server = std::make_shared<EventServer>();
  std::shared_ptr<ServiceManager> service_manager = std::make_shared<ServiceManager>();
  std::shared_ptr<CoreContext> context;
  
  void SetUp() override {
    plog::init(plog::debug, "mock.log");
    context = std::make_shared<CoreContext>(plog::get(), event_server, service_manager, TEST_LIB_DIR);
    
    service_manager->load(context);
  }

};

TEST_F(SquidbotTest, main) {
  sleep(5);
}