#include "obs.h"
#include "events.h"

void OBS::load() { buffer_client->subscribe(TEST_EVENT_TYPE); }
void OBS::unload() {}
void OBS::run() {
  PLOGD << "OBS Service Running with client " << buffer_client << std::endl;
  is_running = true;
  while(is_running) {
    Event front_event = buffer_client->receive();
    if (front_event != NULL_EVENT) {
      switch(front_event.type) {
        case TEST_EVENT_TYPE:
          buffer_client->send(front_event);
          is_running = false;
          break;
      }
    }
  }
}

extern "C" OBS* create(Context_t* context) {
  auto p = new OBS(context);
  return p;

}

extern "C" void destroy(OBS * p) {
  delete p;
}