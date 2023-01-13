#include <gmock/gmock.h>

#include "service.h"
#include "service_loader.h"
#include "service_manager.h"
#include "servicelib.h"

#include "events.h"

class ServiceTestEventClient : public EventClientBase {
public:
  MOCK_METHOD(void, subscribe, (EventType));
  MOCK_METHOD(void, send, (Event));
  MOCK_METHOD(const Event, receive, ());
};

class ServiceTestEventServer : public EventServerBase {
public:
  MOCK_METHOD(std::shared_ptr<EventClientBase>, create_client, ());
};

class ServiceTestServiceManager : virtual public ServiceManagerBase {
public:
  ServiceTestServiceManager() {}
  ~ServiceTestServiceManager() {}
  MOCK_METHOD(void, register_service,
              (std::type_index, std::shared_ptr<ServiceBase>));

  MOCK_METHOD(void, load, (std::shared_ptr<CoreContext>));
  void stop_service(std::type_index __i, std::shared_ptr<Service<ServiceBase>> service) {
    service->stop();
    service.reset();
  }
};

/*
ServiceTest MockService Classes
 */

//
class ServiceTestServiceData : virtual public ServiceDataBase {
public:
  ~ServiceTestServiceData() {}
};


class ServiceTestPluginInterface : virtual public ServiceInterfaceBase {
public:
  ServiceTestPluginInterface(std::shared_ptr<ServiceTestServiceData> data) : data(data) {}
  ~ServiceTestPluginInterface() {}
  void test() { std::cerr << "{<[--MEOW--]>}" << std::endl; }
protected:
  std::shared_ptr<ServiceTestServiceData> data;

};

class ServiceTestExternalInterface : virtual public ServiceInterfaceBase {
  ~ServiceTestExternalInterface() {}
};

class ServiceTestRunActionContext : virtual public RunActionContextBase {
public:
  ServiceTestRunActionContext(std::shared_ptr<EventClientBase> event_client,
                              std::shared_ptr<ServiceTestServiceData> data)
      : event_client(event_client), data(data) {}
  ~ServiceTestRunActionContext() {}
  std::shared_ptr<EventClientBase> event_client;
  std::shared_ptr<ServiceTestServiceData> data;
};

class ServiceTestRunAction : virtual public RunActionBase {
public:
  typedef ServiceTestRunActionContext context_t;
  ServiceTestRunAction(std::shared_ptr<ServiceTestRunActionContext> _context) {
    event_client = _context->event_client;
  }
  MOCK_METHOD(void, run_action, (), (override));
  std::shared_ptr<EventClientBase> event_client;
};

class ServiceTestMockService {
public:
  typedef ServiceTestRunAction run_action_t;
  typedef ServiceTestServiceData data_t;
  typedef ServiceTestPluginInterface plugin_interface_t;
  typedef ServiceTestExternalInterface external_interface_t;
};