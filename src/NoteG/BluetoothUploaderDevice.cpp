#include "BluetoothUploaderDevice.h"

void BluetoothUploaderDevice::onMessage(void(*message_listener)(int size, char* str)) {
  this->message_listener = message_listener;
}

void BluetoothUploaderDevice::print(String message) {
//  Serial.println("test");
  (*(this->bluetooth_serial)).print(message);
}

void BluetoothUploaderDevice::reset() {
    Serial.println("Emit bluetooth reset progress...");
    this->at = 0;
    this->at_read = 0;
}

void BluetoothUploaderDevice::processMessage(int size, char* str) {
//  Serial.println(size);

  String message(str);
  if(message.substring(0, 2) == "OK") {
    this->at_read += 1;
    Serial.println(message);
    if(message.substring(0, 7) == "OK+LOST") {
      this->reset();
    }
    return;
  }
  this->message_listener(size, str);
}
