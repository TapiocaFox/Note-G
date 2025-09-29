#ifndef NoteGGameDevice_H_
#define NoteGGameDevice_H_
#include "EventDevice.h"
#include "BluetoothUploaderDevice.h"
#include "Display.h"

class NoteGGameDevice: public EventDevice {
  public:
    NoteGGameDevice() {
    };
    int score = 0;
    bool startPlayingMusic = false;
    
    void setup() override;

    bool loop(int latest_valid_loop_millisec_offset) override {
      bool abandon_this_loop = false;
      return abandon_this_loop;
    };

    void buttonInput(uint8_t channel, uint8_t state);

    void importSheetMusic(int size, char* str);
    // Stop game, import then start game.

    void pushNotification(String title, String message); // loading..., uploading...
    // Do not interupt game.

    bool showComfirmDialog(String title, String question); // yes no. E.g. import new sheet music?
    // Pause game then resume.

    bool showAlertDialog(String title, String question); // ok. E.g. Please import music sheet to start game. E.g. Your score is 12000.
    // Pause game then resume.

    void startGame();
    void playMusic();

    void stopGame();

    void pauseGame();

    void resumeGame();

    void lookForBars();
    void DrawFallingBar();

    class Bar{
      public:
        Bar() = default;
        Bar(uint8_t ch);
        ~Bar() = default;
      
        unsigned long InitTime = 0;
        uint16_t lastPos = 60;
        uint8_t channel;
        void draw(uint16_t pixel_per_sec);
        unsigned long hit(unsigned long offsett);
    };
    
  private:
    int state = 0;
    // 0 No sheet music data. In menu. Show import alert.
    // 1 Has sheet music data. In menu. Two conditions: 1. Game initially hasn't started yet 2. After game finished. Game became non-started(also showing your score alert dialog).
    // 2 Has sheet music data. Playing.
    // 3 Has sheet music data. Paused.
    unsigned long debounceDelay = 50;  
    uint8_t buttonState[5] = {0,0,0,0,0};
    uint8_t lastButtonState[5] = {0,0,0,0,0};
    unsigned long lastDebounceTime[5] = {0,0,0,0,0};  // the last time the output pin was toggled
    Bar *BarPool[4][10] = {{NULL}};
    uint8_t BarPool_front[4] = {0,0,0,0};
    uint8_t BarPool_size[4] = {0,0,0,0};
    int sheetSize;
    char *pSheet = NULL;
    unsigned long gameStartTime;
    unsigned long musicTime = 0;
    unsigned long barTime = 0;
    uint16_t PC = 0; // actually is the sheet index counter for playing music. It acts like program counter, adding 4 every srep, so I made it "PC" haha.
    uint16_t bPC = 0; //PC for bar
    bool rest = false;
    unsigned long lastDrawTime = 0;
    // MCUFRIEND_kbv *TTT;
    // uint8_t channelHeight = 35; //how many time units?
    bool aboutToEnd = false;
    

    // setup title reset score
    void initGame();
    void initBarPC(uint16_t &pc, unsigned long &barTime);
    void addBar(char instruction);
    
};

#endif
