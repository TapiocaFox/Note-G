#ifndef NoteGGameDevice_H_
#define NoteGGameDevice_H_
#include "EventDevice.h"
#include "BluetoothUploaderDevice.h"

class NoteGGameDevice: public EventDevice {
  public:
    NoteGGameDevice() {
    };

    void setup() override {
      (*setup_finished_listener)();
    };

    bool loop(int latest_valid_loop_millisec_offset) override {
      bool abandon_this_loop = false;
      return abandon_this_loop;
    };

    void startGame();

    void stopGame();

    void importSheetMusic(int bytes_length, byte *bytes);
};

#endif
