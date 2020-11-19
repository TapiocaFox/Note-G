#include "EventDevice.h"

void EventDevice::setup() {
  (*setup_finished_listener)();
}

bool EventDevice::loop(int latest_valid_loop_millisec_offset) {
  bool abandon_this_loop = false;
  return abandon_this_loop;
}

void EventDevice::onSetupFinished(void (*setup_finished_listener)()) {
  this->setup_finished_listener = setup_finished_listener;
}
