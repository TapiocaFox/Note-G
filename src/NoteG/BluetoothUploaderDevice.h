#ifndef BluetoothUploaderDevice_H_
#define BluetoothUploaderDevice_H_
#include <Arduino.h>
#include "EventDevice.h"
#include <SoftwareSerial.h>

class BluetoothUploaderDevice: public EventDevice {
    public:
      void setup(int serial_number, SoftwareSerial *bt) {
        (*bt).begin(serial_number);
        this->bluetooth_serial = bt;
        (*setup_finished_listener)();
        this->print("AT+RENEW");
        this->print("AT+RESET");
        this->print("AT+IMME0");
        this->print("AT+ROLE0");
        this->print("AT+ADDR?");
      };
      bool loop(int latest_valid_loop_millisec_offset) override {
        if(latest_valid_loop_millisec_offset > 200) {
          String str = "";
          SoftwareSerial bluetooth_serial = (*(this->bluetooth_serial));
          while((*(this->bluetooth_serial)).available()) {
            char c = (*(this->bluetooth_serial)).read();
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
      void print(String string);
    private:
      void(*message_listener)(String message);
      SoftwareSerial *bluetooth_serial;
};

#endif
