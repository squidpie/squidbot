#include <gtest/gtest.h>

#include <thread>
#include <unistd.h>

#include "squidbot.h"

class SquidbotTest: public testing::Test {
protected:

  const std::string TEST_LIB_DIR = "/home/squidpie/Repos/squidbot/targets/lib/test/";
  ServiceManager mock_manager = ServiceManager(TEST_LIB_DIR);
  ServiceContainer_t * mock_container;
  EventServer mock_buffer_server;

  void SetUp() override {
    plog::init(plog::debug, "mock.log");
    auto context = Context_t {plog::get(), &mock_buffer_server};
    mock_container = new ServiceContainer("mockservice", &context);
  }

};

TEST_F(SquidbotTest, Squidbot_main) {
  ASSERT_NO_THROW(mock_manager.load(mock_container));
  std::thread mock_thread(&ServiceContainer::run, mock_container);
  mock_thread.join();

}