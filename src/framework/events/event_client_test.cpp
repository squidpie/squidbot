#include <cstdint>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

#include "event_client.h"
#include "events.h"

class EventClientTest : public testing::Test {
protected:
  std::unique_ptr<EventClient> dut;
  std::shared_ptr<std::mutex> q_lock;
  std::shared_ptr<std::queue<Event>> q;

  void SetUp() override {
    uint_fast64_t id = 1;
    q_lock = std::make_shared<std::mutex>();
    q = std::make_shared<std::queue<Event>>();

    dut = std::make_unique<EventClient>(id, q_lock, q);
  }
};

TEST_F(EventClientTest, tx) {
    Event test_event {.type = TEST_EVENT_TYPE };
    dut->send(test_event);
    EXPECT_TRUE(q->size() == 1);
    EXPECT_TRUE(q->front().source == 1);
    EXPECT_TRUE(q->front().type == TEST_EVENT_TYPE);
}

TEST_F(EventClientTest, rx) {
  Event test_event {.source = 2, .type = TEST_EVENT_TYPE };
  q->push(test_event);
  Event rx_event = dut->receive();
  EXPECT_TRUE(rx_event == test_event);
}