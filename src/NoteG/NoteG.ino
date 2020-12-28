#include <Arduino.h>
#include <SoftwareSerial.h>
#include "EventDevice.h"
#include "BluetoothUploaderDevice.h"
#include "SerialDevice.h"
#include "NoteGGameDevice.h"

EventDevice ed;
SerialDevice sd;
BluetoothUploaderDevice bluetooth_uploader;
NoteGGameDevice note_g;
SoftwareSerial bt(8, 7);

void BluetoothUploaderDeviceMessageListener(int size, char* str) {
//  Serial.println(message);
  note_g.importSheetMusic(size, str);
}

void SDMessageListener(String message) {
  sd.printLine(message);
//  bsd.print(message);
}

void SetupTheRest() {
  Serial.println("Setup successfully.");
}

void NoteGSetupFinishedListener() {
  Serial.println("NoteG setup successfully.");
  SetupTheRest();
}

void BluetoothUploaderSetupFinishedListener() {
  Serial.println("BluetoothUploader setup successfully.");
  note_g.onSetupFinished(NoteGSetupFinishedListener);
  note_g.setup();
}

void SDSetupFinishedListener() {
  bluetooth_uploader.onMessage(BluetoothUploaderDeviceMessageListener);
  bluetooth_uploader.onSetupFinished(BluetoothUploaderSetupFinishedListener);
  bluetooth_uploader.setup(9600, &bt);
}

void beginEventDeviceSetupSerial() {
  sd.onMessage(SDMessageListener);
  sd.onSetupFinished(SDSetupFinishedListener);
  sd.setup(9600);
}

void setup() {
  beginEventDeviceSetupSerial();
}

int bluetooth_uploader_latest_valid_loop_millisec = millis();
int note_g_latest_valid_loop_millisec = millis();
int serial_device_latest_valid_loop_millisec = millis();
void loop() {
  // ed.loop();

  if(!sd.loop(millis() - serial_device_latest_valid_loop_millisec)) {
    serial_device_latest_valid_loop_millisec = millis();
  };
  if(!bluetooth_uploader.loop(millis() - bluetooth_uploader_latest_valid_loop_millisec)) {
    bluetooth_uploader_latest_valid_loop_millisec = millis();
  };
  if(!note_g.loop(millis() - note_g_latest_valid_loop_millisec)) {
    note_g_latest_valid_loop_millisec = millis();
  };
};
