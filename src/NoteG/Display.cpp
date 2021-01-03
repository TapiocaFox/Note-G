#include "Display.h"
MCUFRIEND_kbv tft;

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

void showmsgXY(int x, int y, int sz, int color,const char *msg){
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(sz);
  tft.print(msg);
}

void DrawFallingRect(int lane,int pixel_per_sec){
  
}

void FillRectFast(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
  for(uint16_t i = 0; i < h; i++){
    tft.drawFastHLine(x, y+i, w, color);
  }
}
void InitializeDisplay(){
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
