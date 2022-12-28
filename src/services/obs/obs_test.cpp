#include <gtest/gtest.h>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <chrono>

#include "events.h"
#include "service_manager.h"
#include "obs.h"
#include "utils.h"


class OBSTest: public testing::Test {
protected:
  std::thread* obs_thread;
  std::thread* event_server_thread;
  ServiceContainer_t * dut;
  ServiceManager service_manager;
  EventServer mock_event_server;
  EventClient* mock_client;

  void SetUp() {
    plog::init(plog::debug, "mock.log");
    Context_t context = { plog::get(), &mock_event_server };

    event_server_thread = new std::thread(&EventServer::run, &mock_event_server);

    dut = new ServiceContainer("obs", &context);

    service_manager.load(dut);
    obs_thread = new std::thread(&ServiceContainer::run, dut);
    mock_client = mock_event_server.create_client();
  }

  void TearDown() {
    obs_thread->join();
    mock_event_server.stop();
    event_server_thread->join();
  }
};

TEST_F(OBSTest, OBS_event_rx) {
  sleep(1);
  Event event { .type = TEST_EVENT_TYPE };
  mock_client->send(event);
}

TEST_F(OBSTest, DISABLED_OBS_event_tx) {
  Event event { .type = TEST_EVENT_TYPE };
  ASSERT_EQ(TEST_EVENT_TYPE, event.type);
  obs_thread->join();
}
