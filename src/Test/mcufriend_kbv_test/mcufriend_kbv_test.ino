#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#include <SPI.h>          // f.k. for Arduino-1.5.2
#include "Adafruit_GFX.h"// Hardware-specific library
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define BUTTON A5
#define IDLE_MAX 29
#define IDLE_min 20
#define B1_Max 1024
#define B1_min 1000
#define B2_Max 520
#define B2_min 500
#define B3_Max 220
#define B3_min 200
#define B4_Max 89
#define B4_min 80
#define B5_Max 12
#define B5_min 1

int buttonState = 0;
int lastButtonState;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;  

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int score = 0;
int RectLastPos[4] = {0,0,0,0};
int RectStartTime[4]= {0,0,0,0};
int scroll = 0;
 
int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

void drawLines(){
  tft.drawFastVLine(80, 60, 420, YELLOW);
  tft.drawFastVLine(160, 60, 420, YELLOW);
  tft.drawFastVLine(240, 60, 420, YELLOW);
}

void showmsgXY(int x, int y, int sz, int color,const char *msg)
{
  //int16_t x1, y1;
  //uint16_t wid, ht;
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(sz);
  tft.print(msg);
}

void DrawFallingRect(int lane,int pixel_per_sec){
  // start: 60, width: 100
  if(RectStartTime[lane-1] == 0 ){ 
    RectStartTime[lane-1] =  millis();
    RectLastPos[lane-1] = 60;
    FillRectFast(80*(lane-1)+5, RectLastPos[lane-1], 70, 10, WHITE);
  }
  else{
    //FillRectFast(80*(lane-1)+5, RectLastPos[lane-1], 70, 10, BLACK);
    int lastPos = RectLastPos[lane-1];
    RectLastPos[lane-1] = pixel_per_sec*(millis() - RectStartTime[lane-1])/1000 + 60;
    for(uint16_t i=lastPos; i < RectLastPos[lane-1]; i++){
      tft.drawFastHLine(80*(lane-1)+5, i, 70, BLACK);
    }
    for(uint16_t i=lastPos+10; i < (RectLastPos[lane-1]+10); i++){
      tft.drawFastHLine(80*(lane-1)+5, i, 70, WHITE);
    }
    // FillRectFast(80*(lane-1)+5, RectLastPos[lane-1], 70, 10, WHITE);
  }
}

void FillRectFast(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
  for(uint16_t i = 0; i < h; i++){
    tft.drawFastHLine(x, y+i, w, color);
  }
}

void setup(void) {
    Serial.begin(9600);
    pinMode(BUTTON, INPUT);
    uint32_t when = millis();
    //    while (!Serial) ;   //hangs a Leonardo until you connect a Serial
    if (!Serial) delay(5000);           //allow some time for Leonardo
    Serial.println("Serial took " + String((millis() - when)) + "ms to start");
    //    tft.reset();                 //hardware reset
    uint16_t ID = tft.readID(); //
    Serial.print("ID = 0x");
    Serial.println(ID, HEX);
    if (ID == 0xD3D3) ID = 0x9481; // write-only shield
//    ID = 0x9329;                             // force ID
    tft.begin(ID);
    tft.fillScreen(BLACK);
    tft.setRotation(0);
    drawLines();
    showmsgXY(10, 10, 3, RED, "Score: ");
}

void loop(){
  //int value = analogRead(A5);
  //Serial.println(value);
  int value = analogRead(BUTTON);

  int reading;
  if(value <= IDLE_MAX && value >= IDLE_min) reading = 0;
  else if(value <= B1_Max && value >= B1_min) reading = 1;
  else if(value <= B2_Max && value >= B2_min) reading = 2;
  else if(value <= B3_Max && value >= B3_min) reading = 3;
  else if(value <= B4_Max && value >= B4_min) reading = 4;
  else if(value <= B5_Max && value >= B5_min) reading = 5;
  else reading = 666;

  //Serial.println(reading);
  
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState != 0) {
        Serial.print("button ");
        Serial.println(buttonState);
      }
    }
  }
  lastButtonState = reading;

  showmsgXY(290, 0, 1, WHITE, String(freeMemory()).c_str());
  //if (++scroll >= 25) scroll = 0;
  //tft.vertScroll(60, 320, scroll*10);
  DrawFallingRect(1, 300);
  DrawFallingRect(2, 200);
  DrawFallingRect(3, 100);
  DrawFallingRect(4, 50);
}
