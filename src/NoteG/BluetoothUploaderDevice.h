#ifndef BluetoothUploaderDevice_H_
#define BluetoothUploaderDevice_H_
#include <Arduino.h>
#include "EventDevice.h"


class BluetoothUploaderDevice: public EventDevice {
  public:
    void test();
    void setup() override {
      (*setup_finished_listener)();
    };
    bool loop(int latest_valid_loop_millisec_offset) override {
      if(latest_valid_loop_millisec_offset > 2000) {
        Serial.println("test");
        byte bytes[] = "123";
        (*uploaded_listener)(123, bytes);
        return false;
      }
      else {
        return true;
      }
    };
    void onUploaded(void(*uploaded_listener)(int bytes_length, byte *bytes));
  private:
    void(*uploaded_listener)(int bytes_length, byte *bytes);
};

#endif
