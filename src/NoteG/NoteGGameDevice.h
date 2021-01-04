#ifndef NoteGGameDevice_H_
#define NoteGGameDevice_H_
#include "EventDevice.h"
#include "BluetoothUploaderDevice.h"
#include "Display.h"

class NoteGGameDevice: public EventDevice {
  public:
    NoteGGameDevice() {
    };

    void setup() override {
      (*setup_finished_listener)();
      InitializeDisplay();
    };

    bool loop(int latest_valid_loop_millisec_offset) override {
      bool abandon_this_loop = false;
      return abandon_this_loop;
      //show free memory msg
  //showmsgXY(290, 0, 1, WHITE, String(freeMemory()).c_str());
  //Serial.println(freeMemory());
    };

    void importSheetMusic(int size, char* str);
    // Stop game, import then start game.

    void pushNotification(String title, String message); // loading..., uploading...
    // Do not interupt game.

    bool showComfirmDialog(String title, String question); // yes no. E.g. import new sheet music?
    // Pause game then resume.

    bool showAlertDialog(String title, String question); // ok. E.g. Please import music sheet to start game. E.g. Your score is 12000.
    // Pause game then resume.

    void startGame();

    void stopGame();

    void pauseGame();

    void resumeGame();

  private:
    int state = 0;
    // 0 No sheet music data. In menu. Show import alert.
    // 1 Has sheet music data. In menu. Two conditions: 1. Game initially hasn't started yet 2. After game finished. Game became non-started(also showing your score alert dialog).
    // 2 Has sheet music data. Playing.
    // 3 Has sheet music data. Paused.
};

#endif
