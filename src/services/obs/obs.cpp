#include "obs.h"
#include "events.h"

void OBS::load() {}
void OBS::unload() {}
void OBS::run() {
  PLOGD << "OBS Service Running";
  is_running = true;
  Event sub_event { .type = SUBSCRIBE_EVENT_TYPE, .data = TEST_EVENT_TYPE };
  buffer_client->send(sub_event);
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