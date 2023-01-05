#include <cstdint>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <thread>

#include "event_client.h"
#include "event_server.h"
#include "events.h"
#include "runner.h"

class ServiceTestMockEventClient : virtual public EventClientBase {
public:
  MOCK_METHOD(void, subscribe, (EventType));
  MOCK_METHOD(void, send, (Event));
  MOCK_METHOD(const Event, receive, ());
};

class EventServerRunActionTest : public testing::Test {
protected:
  ClientMap_t mock_client_map;
  SubMap_t mock_sub_map;

  std::shared_ptr<EventServerRunActionContext> dut_run_action_context =
      std::make_shared<EventServerRunActionContext>(&mock_client_map,
                                                    &mock_sub_map);

  EventServerRunAction dut = EventServerRunAction(dut_run_action_context);

  std::shared_ptr<std::mutex> qlock_a = std::make_shared<std::mutex>();
  std::shared_ptr<std::queue<Event>> q_a = std::make_shared<std::queue<Event>>();
  uint_fast64_t id_a = 1;

  std::shared_ptr<std::mutex> qlock_b = std::make_shared<std::mutex>();
  std::shared_ptr<std::queue<Event>> q_b = std::make_shared<std::queue<Event>>();
  uint_fast64_t id_b = 2;

  void SetUp() override {
    mock_client_map[id_a] = std::make_pair(qlock_a, q_a);
    mock_client_map[id_b] = std::make_pair(qlock_b, q_b);

    Event test_event = Event{
        .source = id_a, .type = SUBSCRIBE_EVENT_TYPE, .data = TEST_EVENT_TYPE};
    q_a->push(test_event);
    dut.run_action();
  }

  void TearDown() override {}
};

TEST_F(EventServerRunActionTest, subscribe) {
  EXPECT_TRUE(q_a->size() == 0);
  EXPECT_TRUE(mock_sub_map.size() == 1);
  EXPECT_TRUE(mock_sub_map[TEST_EVENT_TYPE].front() == id_a);
}

TEST_F(EventServerRunActionTest, route) {
  Event route_test_event = Event{.source = id_b, .type = TEST_EVENT_TYPE};
  q_b->push(route_test_event);
  dut.run_action();
  EXPECT_TRUE(q_b->size() == 0);
  EXPECT_TRUE(q_a->size() == 1);
  EXPECT_TRUE(q_a->front() == route_test_event);
}