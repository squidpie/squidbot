#include <gtest/gtest.h>
#include <thread>

#include "event_server.h"
#include "events.h"

class EventsTest : public testing::Test {
protected:
  EventServer dut_server;
};

TEST_F(EventsTest, null_event) {
  auto dut_client = dut_server.create_client();
  Event rx_event = dut_client->receive();
  EXPECT_EQ(rx_event, NULL_EVENT);
}

TEST_F(EventsTest, tx_rx) {
  auto dut_client_a = dut_server.create_client();
  auto dut_client_b = dut_server.create_client();
  EXPECT_FALSE(dut_client_a == NULL);
  EXPECT_FALSE(dut_client_b == NULL);
  dut_server.start();
  dut_client_a->subscribe(TEST_EVENT_TYPE);

  sleep(1);
  Event tx_event{.type = TEST_EVENT_TYPE};
  dut_client_b->send(tx_event);

  sleep(1);
  Event rx_event = dut_client_a->receive();
  EXPECT_EQ(tx_event.type, rx_event.type);

  dut_server.stop();
}
