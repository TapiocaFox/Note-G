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
//        Serial.println("test");
        (*setup_finished_listener)();
      };
      bool loop(int latest_valid_loop_millisec_offset) override {
//        Serial.println("test");
          if(latest_valid_loop_millisec_offset > 180) {
            if(this->at < 6 && this->at == this->at_read){
//            Serial.println(this->at);
            this->at += 1;
            if(this->at == 1) {
              this->print("AT+RENEW");
            }
            if(this->at == 2) {
              this->print("AT+RESET");
            }
            if(this->at == 3) {
              this->print("AT+IMME0");
            }
            if(this->at == 4) {
              this->print("AT+ROLE0");
            }
            if(this->at == 5) {
              this->print("AT+NAMENOTE-G");
            }
            if(this->at == 6) {
              this->print("AT+ADDR?");
            }
            return false;
          }
          
          char str[64];
          int size = 0;
          while((*(this->bluetooth_serial)).available()) {
            char c = (*(this->bluetooth_serial)).read();
            
//            Serial.println((char)c);
            str[size] = c;
            size += 1;
            delay(5);
          }
          str[size] = '\0';
//          Serial.println(size);
          if(size) {
            this->processMessage(size, str);
          }
          return false;
        }
        else {
          return true;
        }
      };
      void onMessage(void(*message_listener)(int size, char* str));
      void print(String string);
    private:
      int at = 0;
      int at_read = 0;
      int packet_left = 0;
      char message[1024];
      int message_size = 0;
      void reset();
      void(*message_listener)(int size, char* str);
      void processMessage(int size, char* str);
      SoftwareSerial *bluetooth_serial;
};

#endif
