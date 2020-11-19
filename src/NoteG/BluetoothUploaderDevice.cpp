#include "BluetoothUploaderDevice.h"

void BluetoothUploaderDevice::test() {
  Serial.println("123 successfully.");
}

void BluetoothUploaderDevice::onUploaded(void(*uploaded_listener)(int bytes_length, byte *bytes)) {
  this->uploaded_listener = uploaded_listener;
}
