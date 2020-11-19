#ifndef EventDevice_H_
#define EventDevice_H_

class EventDevice {
  public:
    EventDevice() {

    };
    virtual void setup();
    virtual bool loop(int latest_valid_loop_millisec_offset);
    void onSetupFinished(void (*setup_finished_listener)());
  protected:
    void (*setup_finished_listener)();
};

#endif
