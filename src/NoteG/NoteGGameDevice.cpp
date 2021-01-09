#include "NoteGGameDevice.h"
#include "Pitch.h"

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
        if(channel == 1) startGame();
        else stopGame();
      }
    }
  }
  lastButtonState[channel] = state;
}

void NoteGGameDevice::startGame(){
  // draw bar need to look ahead
  // first n bar won't be drawn
  //      A channel is k  time unit long. Each time unit is 420/k     pixel long. Falling speed is (420/k)px/(TimeUnit)ms
  // e.g. A channel is 35 time unit long. Each time unit is 420/35=12 pixel long. Falling speed is 12px/25ms = 480px/sec
  gameStartTime = millis();
  startPlayingMusic = true;
  musicTime = 0;
  PC = 7+pSheet[6];
}

void NoteGGameDevice::stopGame(){
  startPlayingMusic = false;
  noTone(13);
}

void NoteGGameDevice::playMusic(){
  if(!startPlayingMusic) return;
  if(millis() - gameStartTime > musicTime){
    if(!rest){
      tone(13, NOTE[pSheet[PC]]);
      musicTime += pSheet[PC+1]*pSheet[0];
      rest = true;
      Serial.print("note: ");
      Serial.print(NOTE[pSheet[PC]]);
      Serial.print(", PC: ");
      Serial.print(PC);
      Serial.print(", musicTime: ");
      Serial.println(musicTime);
    }
    else if(rest){
      tone(13, 0);
      musicTime += pSheet[PC+2]*pSheet[0];
      PC += 4;
      rest = false;
      Serial.print("note: ");
      Serial.print(NOTE[pSheet[PC]]);
      Serial.print(", PC: ");
      Serial.print(PC);
      Serial.print(", musicTime: ");
      Serial.println(musicTime);
    }
  }
  if(PC > sheetSize) stopGame();
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
