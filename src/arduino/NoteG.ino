#include <Arduino.h>
#include <SoftwareSerial.h>
#include "EventDevice.h"
#include "BluetoothUploaderDevice.h"
#include "SerialDevice.h"
#include "NoteGGameDevice.h"

#define BUTTON_1 A5
#define BUTTON_2 12
//#define BUTTON_3 13
//#define BUTTON_4 1
#define SPEAKER 13

EventDevice ed;
SerialDevice sd;
BluetoothUploaderDevice bluetooth_uploader;
NoteGGameDevice note_g;
SoftwareSerial bt(10, 11);

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
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  //pinMode(BUTTON_3, INPUT);
  //pinMode(BUTTON_4, INPUT);
}

int bluetooth_uploader_latest_valid_loop_millisec = millis();
int note_g_latest_valid_loop_millisec = millis();
int serial_device_latest_valid_loop_millisec = millis();
void loop() {
  // ed.loop();

  //show free memory msg
  //showmsgXY(290, 0, 1, WHITE, String(freeMemory()).c_str());
  //Serial.println(freeMemory());

  if(!sd.loop(millis() - serial_device_latest_valid_loop_millisec)) {
    serial_device_latest_valid_loop_millisec = millis();
  };
  if(!bluetooth_uploader.loop(millis() - bluetooth_uploader_latest_valid_loop_millisec)) {
    bluetooth_uploader_latest_valid_loop_millisec = millis();
  };
  if(!note_g.loop(millis() - note_g_latest_valid_loop_millisec)) {
    note_g_latest_valid_loop_millisec = millis();
  };

  // button detection
  note_g.buttonInput(1, analogRead(BUTTON_1) > 512 ? HIGH : LOW);
  note_g.buttonInput(2, digitalRead(BUTTON_2));
  //note_g.buttonInput(3, digitalRead(BUTTON_3));
  //note_g.buttonInput(4, digitalRead(BUTTON_4));

  note_g.playMusic();
  note_g.lookForBars();
  note_g.DrawFallingBar();
};
