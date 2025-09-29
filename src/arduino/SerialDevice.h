#ifndef SerialDevice_H_
#define SerialDevice_H_
#include <Arduino.h>
#include "EventDevice.h"
#include <SoftwareSerial.h>

class SerialDevice: public EventDevice {
  public:
    void setup(int serial_number) {
      Serial.begin(serial_number);
      (*setup_finished_listener)();
    };
    bool loop(int latest_valid_loop_millisec_offset) override {
      if(latest_valid_loop_millisec_offset > 200) {
        String str = "";
        while(Serial.available()) {
          char c = Serial.read();
          if(c != 10 && c!= 13) {
            str += c;
          }
        }
        if(str != "") {
          this->message_listener(str);
        }
        return false;
      }
      else {
        return true;
      }
    };
    void onMessage(void(*message_listener)(String message));
    void printLine(String string);
  private:
    void(*message_listener)(String message);
};

void SerialDevice::onMessage(void(*message_listener)(String message)) {
  this->message_listener = message_listener;
}

void SerialDevice::printLine(String message) {
  Serial.println(message);
}

#endif
