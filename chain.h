#ifndef chain_h
#define chain_h

#include "Arduino.h"

class ChainClass {
  public:
    ChainClass();
    void begin();
    void init();
    void action(uint8_t);
    void update();
    void setView(uint8_t,uint8_t);
  private:
    bool selectionMode;
    uint8_t view_offset;
    uint8_t * cell;
    uint8_t view_chain;
    uint8_t view_track;

    void actionCue(uint8_t);
    void actionSelect(uint8_t);
    void actionCopy(uint8_t);
    void actionPaste(uint8_t);
    void actionCreate(uint8_t);
    void actionEdit(uint8_t);
    void actionDelete(uint8_t);
    void actionMove(uint8_t);
    void setData();
    void setCell();
};

extern ChainClass Chain;
#endif
