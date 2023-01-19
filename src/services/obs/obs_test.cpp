#include "gmock/gmock.h"
#include <cstdint>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <chrono>

#include "events.h"
#include "service_manager.h"
#include "obs.h"
#include "utils.h"

class MockEventClient : public EventClientBase {
  public:
    //MockEventClient(uint_fast64_t id, std::mutex* qlock, std::queue<Event>* q) : EventClient(id, qlock, q) {}
    MOCK_METHOD(void, subscribe, (EventType));
    MOCK_METHOD(void, send, (Event));
    MOCK_METHOD(const Event, receive, ());
};

class MockEventServer : public EventServerBase {
  public:
    MOCK_METHOD(EventClientBase*, create_client, ());
};

const uint MOCK_EVENT_CLIENT_ID_A = 1;
const uint MOCK_EVENT_CLIENT_ID_B = 2;

class OBSTest: public testing::Test {
protected:
  std::thread* obs_thread;
  std::thread* event_server_thread;
  ServiceBase * dut;

  std::mutex* mock_qlock = new std::mutex;
  std::queue<Event>* mock_q = new std::queue<Event>;
  MockEventServer mock_event_server;
  testing::NiceMock<MockEventClient> mock_event_client_A;

  create_t *create_service;
  destroy_t *destroy_service;
  const std::string lib_path = "/home/squidpie/Repos/squidbot/targets/lib/libobs.so";


  void SetUp() {
    Context_t context = { plog::get(), &mock_event_server };

    ON_CALL(mock_event_server, create_client())
        .WillByDefault(testing::Return(&mock_event_client_A));

    ON_CALL(mock_event_client_A, receive())
        .WillByDefault(testing::Invoke(
            [this](){ Event e = mock_q->size() ? mock_q->front() : NULL_EVENT;
                      try { mock_q->pop(); } catch(...) {}
                      return e;}));

    ON_CALL(mock_event_client_A, send(testing::_))
        .WillByDefault(testing::Invoke(
            [this](Event e){ e.source = MOCK_EVENT_CLIENT_ID_A; mock_q->push(e);}));

    ON_CALL(mock_event_client_A, subscribe(testing::_))
        .WillByDefault(testing::Return());

    void* handle = dlopen(lib_path.c_str(), RTLD_LAZY);
    const char* dlsym_error = dlerror();
    if (!handle) {
      LOGE << "Failed to open lib_path: " << lib_path.c_str();
      throw ServiceLibError();
    }

    create_service = (create_t*) dlsym(handle, "create");
    dlsym_error = dlerror();
    if (dlsym_error) {
      LOGE << "Failed to link create function";
      throw ServiceLibError();
    }

    destroy_service = (destroy_t*) dlsym(handle, "destroy");
    dlsym_error = dlerror();
    if (dlsym_error) {
      LOGE << "Failed to link destroy function";
      throw ServiceLibError();
    }

    EXPECT_CALL(mock_event_server, create_client).Times(testing::Exactly(1));
    dut = create_service(&context);
    ASSERT_FALSE(dut == NULL);
    obs_thread = new std::thread(&ServiceBase::run, dut);
  }

  void TearDown() {
    obs_thread->join();
    destroy_service(dut);
  }
};

TEST_F(OBSTest, OBS_event_rx_tx) {
  EXPECT_CALL(mock_event_client_A, receive);
  EXPECT_CALL(mock_event_client_A, send);
  Event event { .source=MOCK_EVENT_CLIENT_ID_B, .type = TEST_EVENT_TYPE };
  mock_q->push(event);
}