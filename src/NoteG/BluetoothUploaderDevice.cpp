#include "BluetoothUploaderDevice.h"

void BluetoothUploaderDevice::onMessage(void(*message_listener)(String message)) {
  this->message_listener = message_listener;
}

void BluetoothUploaderDevice::print(String message) {
  (*(this->bluetooth_serial)).print(message);
}

