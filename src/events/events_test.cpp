#include <gtest/gtest.h>
#include <thread>

#include "events.h"

class EventsTest: public testing::Test {
protected:
    EventServer dut_server;

    void SetUp() {}
};

TEST_F(EventsTest, EventsBufferClient_null_event) {
  auto dut_client = dut_server.create_client();
  Event rx_event = dut_client->receive();
  EXPECT_EQ(rx_event, NULL_EVENT);
}


TEST_F(EventsTest, EventBufferServer_tx_rx) {
    auto dut_client_a = dut_server.create_client();
    auto dut_client_b = dut_server.create_client();
    EXPECT_FALSE(dut_client_a == NULL);
    EXPECT_FALSE(dut_client_b == NULL);

    std::thread dut_server_thread(&EventServer::run, &dut_server);

    dut_client_a->subscribe(TEST_EVENT_TYPE);
    sleep(1);
    Event tx_event { .type=TEST_EVENT_TYPE };
    dut_client_b->send(tx_event);

    Event rx_event = dut_client_a->receive();
    while (rx_event == NULL_EVENT) {
      rx_event = dut_client_a->receive();
    }
    EXPECT_EQ(TEST_EVENT_TYPE, rx_event.type);

    dut_server.stop();
    dut_server_thread.join();
}
