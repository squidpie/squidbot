#include "squidbot.h"
#include "service.h"
#include "service_manager.h"

class TestData : virtual public ServiceDataBase {
  ~TestData() {}
};

class TestRunActionContext : virtual public RunActionContextBase {
public:
  TestRunActionContext(std::shared_ptr<EventClientBase> event_client,
                       std::shared_ptr<TestData> data)
      : event_client(event_client), data(data) {}
  ~TestRunActionContext() {}
  std::shared_ptr<EventClientBase> event_client;
  std::shared_ptr<TestData> data;
};

class TestRunAction : virtual public RunActionBase {
public:
  typedef TestRunActionContext context_t;
  TestRunAction(std::shared_ptr<TestRunActionContext> _context) {}
  ~TestRunAction() {}
  void run_action() {
    PLOGD << "MEOW";
    sleep(5);
  }
};

class TestPluginInterface : virtual public ServiceInterfaceBase {
public:
  TestPluginInterface(std::shared_ptr<TestData> data) : data(data) {}
  ~TestPluginInterface() {}
  protected:
    std::shared_ptr<TestData> data;
};

class TestExternalInterface : virtual public ServiceInterfaceBase {
public:
  ~TestExternalInterface() {}
};

class TestService {
public:
  typedef TestRunAction run_action_t;
  typedef TestPluginInterface plugin_interface_t;
  typedef TestExternalInterface external_interface_t;
  typedef TestData data_t;
};

int main(int argc, char **argv) {
  plog::init(plog::debug, "squidbot.log");
  auto event_server = std::make_shared<EventServer>();
  std::shared_ptr<ServiceManager> service_manager;
  std::shared_ptr<ServiceContext> service_context = std::make_shared<ServiceContext>(plog::get(), event_server, service_manager, LIB_DIR);
  Service<TestService> s = Service<TestService>(service_context);
  s.start();
  sleep(1);
  s.stop();
}