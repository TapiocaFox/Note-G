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
    Serial.print(""); Serial.println(""); 
    RectLastPos[lane-1] = 60;
    FillRectFast(80*(lane-1)+5, RectLastPos[lane-1], 70, 10, WHITE);
  }
  else{
    FillRectFast(80*(lane-1)+5, RectLastPos[lane-1], 70, 10, BLACK);
    RectLastPos[lane-1] = pixel_per_sec*(millis() - RectStartTime[lane-1])/1000 + 60;
    FillRectFast(80*(lane-1)+5, RectLastPos[lane-1], 70, 10, WHITE);
  }
}

void FillRectFast(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
  for(uint16_t i = 0; i < h; i++){
    tft.drawFastHLine(x, y+i, w, color);
  }
}

void setup(void) {
    Serial.begin(9600);
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
  showmsgXY(290, 0, 1, WHITE, String(freeMemory()).c_str());
  //if (++scroll >= 25) scroll = 0;
  //tft.vertScroll(60, 320, scroll*10);
  DrawFallingRect(1, 100);
  DrawFallingRect(2, 100);
  DrawFallingRect(3, 100);
  DrawFallingRect(4, 100);
}
