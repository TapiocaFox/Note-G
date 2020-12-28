#include <Arduino.h>
#include <SoftwareSerial.h>
#include "EventDevice.h"
#include "BluetoothUploaderDevice.h"
#include "NoteGGameDevice.h"

EventDevice ed;
BluetoothUploaderDevice bluetooth_uploader;
// NoteGGameDevice note_g;
NoteGGameDevice note_g;
SoftwareSerial bt(8, 7);

void BluetoothUploaderDeviceMessageListener(String message) {
  Serial.println(message);
  note_g.importSheetMusic(message);
}

void SetupTheRest() {
  bluetooth_uploader.onMessage(BluetoothUploaderDeviceMessageListener);
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

void EDSetupFinishedListener() {
  bluetooth_uploader.onSetupFinished(BluetoothUploaderSetupFinishedListener);
  bluetooth_uploader.setup(9600, &bt);
}

void beginEventDeviceSetupSerial() {
  ed.onSetupFinished(EDSetupFinishedListener);
  ed.setup();
}

void setup() {
  Serial.begin(9600);
  beginEventDeviceSetupSerial();
}

int bluetooth_uploader_latest_valid_loop_millisec = millis();
int note_g_latest_valid_loop_millisec = millis();
void loop() {
  // ed.loop();

  if(!bluetooth_uploader.loop(millis() - bluetooth_uploader_latest_valid_loop_millisec)) {
    bluetooth_uploader_latest_valid_loop_millisec = millis();
  };
  if(!note_g.loop(millis() - note_g_latest_valid_loop_millisec)) {
    note_g_latest_valid_loop_millisec = millis();
  };
};
