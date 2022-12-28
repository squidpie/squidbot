#include "squidbot.h"
#include "service_manager.h"

int main() {
  plog::init(plog::debug, "squidbot.log"); // make this configurable
  EventServer event_server;

  EventClient* client_a = event_server.create_client();
  EventClient* client_b = event_server.create_client();
  EventClient* client_c = event_server.create_client();

  Context_t context = Context_t { plog::get(), &event_server };

  ServiceManager service_manager;
  ServiceContainer obs = ServiceContainer("obs", &context);
  service_manager.load(&obs);
  std::thread obs_thread(&ServiceContainer::run, obs);

  EventClient* test_client = event_server.create_client();
  Event stop_obs_event { .type = TEST_EVENT_TYPE };
  test_client->send(stop_obs_event);

  obs_thread.join();
}