#ifndef joypad_h
#define joypad_h

#include "Arduino.h"

#define J_A 0x01
#define J_B 0x02
#define J_START 0x04
#define J_SELECT 0x08
#define J_RIGHT 0x10
#define J_DOWN 0x20
#define J_UP 0x40
#define J_LEFT 0x80

class JoypadClass {
  public:
    int8_t pad[2];
    JoypadClass();
    void begin(uint8_t,uint8_t,uint8_t,uint8_t,uint8_t,uint8_t,uint8_t,uint8_t);
    boolean available();
    uint8_t read();
    void setRepeat(unsigned long, unsigned long);
    boolean isA(uint8_t);
    boolean isB(uint8_t);
    boolean isStart(uint8_t);
    boolean isSelect(uint8_t);
    boolean isPad(uint8_t);
    boolean isRight(uint8_t);
    boolean isDown(uint8_t);
    boolean isUp(uint8_t);
    boolean isLeft(uint8_t);
    boolean isRepeat();
    boolean hasA(uint8_t);
    boolean hasB(uint8_t);
    boolean hasStart(uint8_t);
    boolean hasSelect(uint8_t);
    boolean hasPad(uint8_t);
    boolean hasRight(uint8_t);
    boolean hasDown(uint8_t);
    boolean hasUp(uint8_t);
    boolean hasLeft(uint8_t);
  private:
    uint8_t pins[8];
    uint8_t last_state;
    uint8_t state;
    boolean is_repeat;
    boolean read_waiting;
    unsigned long keyDelay;
    unsigned long keyRepeat = 50;
    unsigned long keyCheck = 100;
    unsigned long timer;
    unsigned long touchTimer; 
    boolean repeatEnable;
    void setPad();
};

extern JoypadClass Joypad;
#endif
