#include "NoteGGameDevice.h"

void NoteGGameDevice::importSheetMusic(int size, char* str) {
  for(int i = 0; i < size; i++) {
    Serial.print(i);
    Serial.print(" >> ");
    Serial.println((byte)(str[i]));
  }
//  Serial.println("bytes_length");
}
