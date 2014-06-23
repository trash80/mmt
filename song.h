#ifndef song_h
#define song_h

#include "Arduino.h"

class SongClass {
  public:
    SongClass();
    void begin();
    void init();
    void action(uint8_t);
    void update();
  private:
    uint8_t view_offset;
    uint8_t * cell;
    
    bool selection_active;
    uint16_t selection_start;
    uint16_t selection_end;
    uint16_t selection_size;

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

extern SongClass Song;
#endif
