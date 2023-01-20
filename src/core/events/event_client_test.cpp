/*
event_client_test.cpp
Copyright (C) 2023  Squidpie
 */

#include <cstdint>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

#include "event_client.h"
#include "events.h"

class EventClientTest : public testing::Test {
protected:
  clientid_t id = 1;
  std::shared_ptr<std::mutex> q_lock = std::make_shared<std::mutex>();
  std::shared_ptr<std::queue<Event>> q = std::make_shared<std::queue<Event>>();

  std::unique_ptr<EventClient> dut =
      std::make_unique<EventClient>(id, q_lock, q);
};

TEST_F(EventClientTest, tx) {
  Event test_event{.type = EVENTS.TEST_EVENT_TYPE};
  dut->send(test_event);
  EXPECT_TRUE(q->size() == 1);
  EXPECT_TRUE(q->front().source == 1);
  EXPECT_TRUE(q->front().type == EVENTS.TEST_EVENT_TYPE);
}

TEST_F(EventClientTest, rx) {
  Event test_event{.source = 2, .type = EVENTS.TEST_EVENT_TYPE};
  q->push(test_event);
  Event rx_event = dut->receive();
  EXPECT_TRUE(rx_event == test_event);
}