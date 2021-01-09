#include "NoteGGameDevice.h"
//#include <cstring>

void NoteGGameDevice::importSheetMusic(int size, char* str) {
  for(int i = 0; i < size; i++) {
    Serial.print(i);
    Serial.print(" >> ");
    Serial.println((byte)(str[i]));
  }
  sheetSize = size;
  pSheet = str;
  initGame();
//  Serial.println("bytes_length");
}

void NoteGGameDevice::buttonInput(uint8_t channel, uint8_t state){
  if (state != lastButtonState[channel]) {
    // reset the debouncing timer
    lastDebounceTime[channel] = millis();
  }
  if ((millis() - lastDebounceTime[channel]) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (state != buttonState[channel]) {
      buttonState[channel] = state;

      // only toggle the LED if the new button state is HIGH
      if (buttonState[channel] != LOW) {
        Serial.print("button ");
        Serial.println(channel);
      }
    }
  }
  lastButtonState[channel] = state;
}

void NoteGGameDevice::startGame(){
  // draw bar need to look ahead
  // first n bar won't be drawn
  //      A channel is a  time unit long. Each time unit is 420/a     pixel long. Falling speed is (420/a)px/(TimeUnit)ms
  // e.g. A channel is 35 time unit long. Each time unit is 420/35=12 pixel long. Falling speed is 12px/25ms = 480px/sec
  playMusic();
}

void NoteGGameDevice::playMusic(){
  
}

void NoteGGameDevice::initGame(){
  //char *title;
  String title = "";
  for(uint8_t i=7; i<7+pSheet[6]; i++){
    title += pSheet[i];
  }
  //strncpy(title, pSheet+7, (size_t)pSheet[6]);
  showmsgXY(10, 5, 2, BLACK, BLACK, "unknown track");
  showmsgXY(10, 5, 2, BLUE, BLACK, title.c_str());
  char *score_str;
  sprintf(score_str, "%d", score);
  showmsgXY(50, 25, 2, WHITE, score_str);
}
