#ifndef cursor_h
#define cursor_h

#include "Arduino.h"

class CursorClass {
  public:
    CursorClass();
    void begin();
    void draw();
    void clearSelection();
    void drawSelection(bool);
    void set(uint8_t, uint8_t);
    void move(int8_t, int8_t);
    void clear();
    void startSelection();
    void endSelection();

    uint8_t col;
    uint8_t row;
    uint8_t size;
    uint16_t pos;
    uint16_t abs_pos;
  private:

    uint8_t real_col;
    uint8_t real_row;
    bool changed;

    uint16_t selection_start_pos;
    uint8_t selection_start_col;
    uint8_t selection_start_row;
    uint8_t selection_start_size;

    uint16_t selection_end_pos;
    uint8_t selection_end_col;
    uint8_t selection_end_row;
    
    bool selection_active;
    bool selection_changed;

    struct mask {
        uint8_t x;
        uint8_t y;
        uint8_t width;
        uint8_t height;
        uint16_t color;
        bool active;
    } masks[2];
};


extern CursorClass Cursor;


#endif
