#include "squidbot.h"
#include "service.h"


class TestRunActionContext : virtual public RunActionContextBase {
  public:
  ~TestRunActionContext() {}
  std::shared_ptr<EventClientBase> event_client;
};

class TestRunAction : virtual public RunActionBase {
  public:
    TestRunAction(std::shared_ptr<TestRunActionContext> _context) {}
  ~TestRunAction() {}
  void run_action() { PLOGD << "MEOW"; sleep(5); }
};

class TestData : virtual public ServiceDataBase {
  ~TestData() {}
};

int main(int argc, char** argv) {
  plog::init(plog::debug, "squidbot.log");
  EventServer event_server;
  ServiceContext service_context {plog::get(), &event_server};
  Service<TestRunAction, TestRunActionContext, TestData> s = Service<TestRunAction, TestRunActionContext, TestData>(service_context);
  s.start();
  sleep(1);
  s.stop();
}