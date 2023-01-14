#include "plugin.h"

void PluginRunAction::run_action() {
  auto rx = client->receive();
  if (rx.type == NULL_EVENT_TYPE) return;
  if (filter->is_trigger(rx)) action->trigger_action(rx);
}