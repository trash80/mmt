/*
 * Cursor class File - maintains on-screen cursor size and position
 * Copyright (C) 2014 Timothy Lamb - trash80.com
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef cursor_h
#define cursor_h

#include "Arduino.h"

class CursorClass {
  public:
    CursorClass();
    void begin();
    void drawBg();
    void drawFg();
    void draw() { drawBg();drawFg(); };
    void clearSelection();
    void drawSelection(bool);
    void set(uint8_t, uint8_t);
    void move(int8_t, int8_t);
    void clear();
    void startSelection();
    void endSelection();
    void moveSelection(int8_t sc, int8_t sr);

    uint8_t col;
    uint8_t row;
    uint8_t size;
    uint16_t pos;
    uint16_t abs_pos;
    
    uint16_t selection_start_pos;
    uint8_t selection_start_col;
    uint8_t selection_start_row;
    uint8_t selection_start_rel_col;
    uint8_t selection_start_rel_row;
    uint8_t selection_start_size;

    uint16_t selection_end_pos;
    uint8_t selection_end_col;
    uint8_t selection_end_row;
  private:

    uint8_t real_col;
    uint8_t real_row;
    bool changed;

    
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
