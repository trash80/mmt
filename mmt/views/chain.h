/*
 * Chain Controller class File
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

#ifndef chain_h
#define chain_h

#include "../globals.h"
#include "../display.h"
#include "../cursor.h"
#include "../joypad.h"
#include "../player.h"
#include "../route.h"

class ChainClass {
  public:
    ChainClass();
    void begin();
    void init();
    void action(uint8_t);
    void update();
    void draw();
    void setView(uint8_t);
  private:
    uint8_t view_offset;
    volatile uint8_t * cell;
    uint8_t view_chain;
    uint8_t view_row;
    uint8_t view_col;
    
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

extern ChainClass Chain;
#endif
