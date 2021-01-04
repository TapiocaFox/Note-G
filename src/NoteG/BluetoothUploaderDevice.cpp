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
      this->packet_left = 0;
      this->message_size = 0;
      this->reset();
    }
    return;
  }
//  Serial.println((int)str[0]);
  if((int)str[0] == 0) {
//    Serial.println((int)str[1]);
    this->packet_left = (int)str[1];
    this->message_size = 0;
  }
  else {
    for(int i=1; i<size; i++) {
      this->message[this->message_size+i-1] = str[i];
    }
    this->message_size += size-1;
    this->packet_left -= 1;
    if(this->packet_left == 0) {
      this->message_listener(this->message_size, this->message);
      this->message_size = 0;
    }
  }
  (*(this->bluetooth_serial)).print("READ OK");
//  
}
