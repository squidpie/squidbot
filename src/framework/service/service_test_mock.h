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
  ServiceTestServiceManager(std::shared_ptr<ServiceContext> context) {}
  ~ServiceTestServiceManager() {}
  MOCK_METHOD(void, register_service,
              (std::type_index, std::shared_ptr<ServiceBase>));

  void stop_service(std::type_index __i, std::shared_ptr<Service<ServiceBase>> service) {
    service->stop();
    service.reset();
  }
};