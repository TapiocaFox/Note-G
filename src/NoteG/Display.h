#ifndef Display_H_
#define Display_H_

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#include <SPI.h>          // f.k. for Arduino-1.5.2
#include "Adafruit_GFX.h"// Hardware-specific library
#include <MCUFRIEND_kbv.h>


// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define ORANGE  0xFC00

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory();
void drawLines();
void showmsgXY(int x, int y, int sz, int color,const char *msg);
void showmsgXY(int x, int y, int sz, int color, int bg_color, const char *msg);
void DrawFallingRect(int lane,int pixel_per_sec);
void FillRectFast(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void InitializeDisplay();

#endif
