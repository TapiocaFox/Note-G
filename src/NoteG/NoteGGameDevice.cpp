#include "NoteGGameDevice.h"
#include "Pitch.h"


void NoteGGameDevice::setup(){
      (*setup_finished_listener)();
      InitializeDisplay();
};

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
        //Serial.print("button ");
        //Serial.println(channel);
        if(channel == 2 && !startPlayingMusic) startGame();
        //else stopGame();
        if( BarPool_size[channel-1] > 0){
          unsigned long HowGooooodAreYou = BarPool[channel-1][BarPool_front[channel-1]]->hit(pSheet[0]*31);
          if(HowGooooodAreYou < pSheet[5]){
            score += 100;
            showmsgXY(80*(channel-1)+10, 450, 2, BLACK, "soso");
            showmsgXY(80*(channel-1)+14, 450, 2, BLACK, "BAD");
            showmsgXY(80*(channel-1)+12, 450, 2, GREEN, BLACK, "GOOD");
          }
          else if(HowGooooodAreYou < pSheet[5]*2){
            score += 50;
            showmsgXY(80*(channel-1)+12, 450, 2, BLACK, "GOOD");
            showmsgXY(80*(channel-1)+14, 450, 2, BLACK, "BAD");
            showmsgXY(80*(channel-1)+10, 450, 2, ORANGE, BLACK, "soso");
          }
          else{
            score += 10;
            showmsgXY(80*(channel-1)+12, 450, 2, BLACK, "GOOD");
            showmsgXY(80*(channel-1)+10, 450, 2, BLACK, "soso");
            showmsgXY(80*(channel-1)+14, 450, 2, RED, BLACK, "BAD");
          }
          showmsgXY(90, 25, 2, WHITE, BLACK, String(score).c_str());
          BarPool_size[channel-1] -= 1;
          for(uint16_t i=BarPool[channel-1][BarPool_front[channel-1]]->lastPos; i < BarPool[channel-1][BarPool_front[channel-1]]->lastPos+10; i++){
            getTFT()->drawFastHLine(80*(channel-1)+5, i, 70, BLACK);
          }
          delete BarPool[channel-1][BarPool_front[channel-1]];
          BarPool_front[channel-1] = (BarPool_front[channel-1]+1)%10;
        }
      }
    }
  }
  lastButtonState[channel] = state;
}

void NoteGGameDevice::startGame(){
  if(pSheet == NULL) return;
  // draw bar need to look ahead
  // first n bar won't be drawn
  //      A channel is k  time unit long. Each time unit is 420/k     pixel long. Falling speed is (420/k)px/(TimeUnit)ms
  // e.g. A channel is 35 time unit long. Each time unit is 420/35=12 pixel long. Falling speed is 12px/25ms = 480px/sec
  // 4 time units below the line, 31 above the line. Look ahead 31 time units.
  gameStartTime = millis();
  lastDrawTime = gameStartTime-1;
  startPlayingMusic = true;
  musicTime = 0;
  score = 0;
  showmsgXY(90, 25, 2, WHITE, BLACK, String("0     ").c_str());
  PC = 7+pSheet[6];
  initBarPC(bPC, barTime);
}

void NoteGGameDevice::stopGame(){
  startPlayingMusic = false;
  noTone(13);
}

void NoteGGameDevice::lookForBars(){
  // I will hard code the amount of time units on channel, time unit height and falling speed for now. It's a must for a 1-day-left project.
  if(!startPlayingMusic) return;
  if(millis() + pSheet[0]*31 - gameStartTime > barTime){
    addBar(pSheet[bPC+3]);
    bPC += 4;
    barTime += (pSheet[bPC+1] + pSheet[bPC+2])*pSheet[0];
    Serial.print(" - bPC: ");
    Serial.println(bPC);
  }
  if(bPC > sheetSize) barTime = 4294967295;
}

void NoteGGameDevice::addBar(char instruction){ // use ref?
  // add bar to coresponding bar pool
  bool channel[4] = {false, false, false, false};
  for(uint8_t i = 0; i < 4; i++)
  {
    channel[i] = ((unsigned char)instruction >> i) & 1;
  }
  for(uint8_t i=0; i<4; i++){
    if(channel[i]){
      BarPool_size[i] += 1;
      if(BarPool_size[i] > 10){
        BarPool_size[i] = 10;
        for(uint16_t j=BarPool[i][BarPool_front[i]]->lastPos; j < BarPool[i][BarPool_front[i]]->lastPos+10; j++){
          getTFT()->drawFastHLine(80*i+5, j, 70, BLACK);
        }
        delete BarPool[i][BarPool_front[i]];
        BarPool_front[i] = (BarPool_front[i]+1)%10;
      }
      
      BarPool[i][(BarPool_front[i]+BarPool_size[i]-1)%10] = new Bar(i+1);
      //if(BarPool_back[i] > BarPool_front[i]) BarPool_front[i] = (BarPool_front[i]+1)%10; // shouldn't happen. just a insurance. but cause mem leak!
    }
  }
}

void NoteGGameDevice::DrawFallingBar(){
  //if(millis() - lastDrawTime < 33) return;
  if(!startPlayingMusic) return;
  for(uint8_t i=0; i<4; i++){
    for(uint8_t j=0; j<BarPool_size[i]; j++){
      BarPool[i][(BarPool_front[i]+j)%10]->draw(12000/pSheet[0]);
    }
  }
  getTFT()->drawFastHLine(0, 437, 320, ORANGE);
  
}

void NoteGGameDevice::initBarPC(uint16_t &pc, unsigned long &barTime){
  pc = 7+pSheet[6];
  barTime = pSheet[pc+1] + pSheet[PC+2];
  while(barTime < 31) {
    pc += 4;
    barTime += pSheet[pc+1] + pSheet[PC+2];
  }
  barTime = barTime*pSheet[0];
}

void NoteGGameDevice::playMusic(){
  if(!startPlayingMusic) return;
  if(millis() - gameStartTime > musicTime){
    if(!rest){
      tone(13, NOTE[pSheet[PC]]);
      musicTime += pSheet[PC+1]*pSheet[0];
      rest = true;
      Serial.print(" - PC: ");
      Serial.println(PC);
      Serial.println(freeMemory());
    }
    else{
      tone(13, 0);
      musicTime += pSheet[PC+2]*pSheet[0];
      PC += 4;
      rest = false;
    }
  }
  if(PC > sheetSize && !aboutToEnd){
    lastDrawTime = millis();
    aboutToEnd = true;
  }
  if(aboutToEnd && millis() - lastDrawTime > 1000) {
    stopGame();
    aboutToEnd = false;
  }
}

void NoteGGameDevice::initGame(){
  //char *title;
  String title = "";
  for(uint8_t i=7; i<7+pSheet[6]; i++){
    title += pSheet[i];
  }
  title += "      ";
  //strncpy(title, pSheet+7, (size_t)pSheet[6]);
  showmsgXY(10, 5, 2, BLACK, BLACK, "unknown track");
  showmsgXY(10, 5, 2, CYAN, BLACK, title.c_str());
  Serial.print("loaded track - ");
  Serial.println(title);
  //char *score_str;
  //sprintf(score_str, "%d", score);
  showmsgXY(90, 25, 2, WHITE, "0");
}

/// Bar
void  NoteGGameDevice::Bar::draw(uint16_t pixel_per_sec){
  if(InitTime == 0){
    InitTime = millis();
    FillRectFast(80*(channel-1)+5, lastPos, 70, 10, WHITE);
  }
  else{
    uint16_t temp = lastPos;
    lastPos = pixel_per_sec*(millis() - InitTime)/1000.0 + 60;
    for(uint16_t i=temp; i < lastPos; i++){
      getTFT()->drawFastHLine(80*(channel-1)+5, i, 70, BLACK);
    }
    for(uint16_t i=temp+10; i < (lastPos+10); i++){
      getTFT()->drawFastHLine(80*(channel-1)+5, i, 70, WHITE);
    }
  }
}

unsigned long NoteGGameDevice::Bar::hit(unsigned long offsett){
  long n = InitTime + offsett - millis();
  return ((n >> 31) ^ n) - (n >> 31);
}

NoteGGameDevice::Bar::Bar(uint8_t ch){channel = ch;}
