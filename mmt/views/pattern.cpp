/*
 * Pattern Controller class File
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

#include "pattern.h"

PatternClass::PatternClass()
{
  view_pattern = 0;
}

void PatternClass::begin()
{
  for(int i=0;i!=8;i++) {
    lastEdit[i] = 0xFF;
  }
  lastEdit[0] = 36;
  lastEdit[1] = 0x64;
  lastEdit[2] = 0;
}

void PatternClass::init()
{
  Display.clearSprites();
  Display.setView(PATTERN_VIEW);

  // Set the pattern # 
  Display.printHex(&Display.screen[10],view_pattern);

  setData();
  setCell();
  Cursor.set(0,0);
}

void PatternClass::update()
{
}

void PatternClass::draw()
{
  Display.clearSprites();
  if(!Player.playing()) return ;

  if(Player.isPatternMode()) {
    if(Player.tracks[0].play_position == view_pattern) {
      Display.setSprite(0,62,4,3+(Player.tracks[0].step_pos%16));
    }
  } else if(Player.isChainMode() && (*Player.tracks[0].chain) == view_pattern) {
    Display.setSprite(0,62,4,3+(Player.tracks[0].step_pos%16));
  } else if(Player.isSongMode() && (*Player.tracks[Song.track()].chain) == view_pattern) {
    Display.setSprite(0,62,4,3+(Player.tracks[Song.track()].step_pos%16));
  }
}

void PatternClass::action(uint8_t input)
{
  switch(input)
  {
    case J_START:
      actionCue(input);
      break;
    case J_SELECT|J_B:
      actionSelect(input);
      break;
    case J_SELECT|J_A:
      actionPaste(input);
      break;
    case J_B:
      actionCopy(input);
      break;
    case J_A:
      actionCreate(input);
      break;
    case J_A|J_B:
      actionDelete(input);
      Joypad.waitClear();
      break;
    case J_A|J_UP:
    case J_A|J_DOWN:
    case J_A|J_LEFT:
    case J_A|J_RIGHT:
      actionEdit(input);
      break;
    case J_UP:
    case J_DOWN:
    case J_LEFT:
    case J_RIGHT:
      actionMove(input);
      break;
    case J_SELECT|J_UP:
      break;
    case J_SELECT|J_DOWN:
      break;
    case J_SELECT|J_LEFT:
      Route.setView(CHAIN_VIEW);
      break;
    case J_SELECT|J_RIGHT:
      Route.setView(TABLE_VIEW);
      break;
  }
}

void PatternClass::actionCue(uint8_t input)
{
  if(!Player.playing() || (Player.playing() && !Player.isPatternMode())) {
    Player.playPattern(view_pattern, 0); // Cursor.row
  } else {
    Player.stop();
  }
}

void PatternClass::actionSelect(uint8_t input)
{
  
  Cursor.startSelection();
  selection_start = Cursor.pos;
  selection_active = true;
  
}

void PatternClass::actionCopy(uint8_t input)
{
  /* @TODO Cleanup / refactor */
  if(!selection_active) return;
  uint8_t selection_start_col,selection_end_col,selection_start_row,selection_end_row;

  selection_end = Cursor.pos < selection_start ? selection_start : Cursor.pos;
  selection_start = Cursor.pos < selection_start ? Cursor.pos : selection_start;
  
  selection_start_col = (selection_start%8);
  selection_start_row = (selection_start/40);
  selection_end_col = (selection_end%8);
  selection_end_row = (selection_end/40);
  if(selection_start_col > selection_end_col) {
    selection_start_col = selection_end_col;
    selection_end_col = (selection_start%8);
  }

  volatile uint8_t * cp = &memory.copy_buffer[0];
  volatile uint8_t * sp = &memory.pattern[view_pattern][selection_start_row][selection_start_col];

  uint8_t mem_count = 0;
  for(uint16_t y=selection_start_row;y<=selection_end_row;y++) {
    for(uint8_t x=selection_start_col;x<=selection_end_col;x++) {
      if(mem_count >= 256) break;
      *cp++ = *sp++;
      mem_count++;
    }
    if(y != 16) sp+=(6-(selection_end_col-selection_start_col));
  }

  /* Move & clip cursor */
  Cursor.clearSelection();
  Cursor.col = selection_start_col;
  Cursor.row = (((uint16_t)selection_end_row) +1)&255;
  if(Cursor.row > 0x0F) {
    Cursor.row = 0x0F;
    setData();
  }
  Cursor.set(Cursor.col,Cursor.row);
  setCell();
  selection_active = false;
}

void PatternClass::actionPaste(uint8_t input)
{
  /* @TODO Cleanup / refactor */
  uint8_t selection_start_col,selection_end_col,selection_start_row,selection_end_row;

  selection_start_col = (selection_start%8);
  selection_start_row = (selection_start/40);
  selection_end_col = (selection_end%8);
  selection_end_row = (selection_end/40);
  if(selection_start_col > selection_end_col) {
    selection_start_col = selection_end_col;
    selection_end_col = (selection_start%8);
  }

  volatile uint8_t * cp = &memory.copy_buffer[0];
  uint8_t x,y;
  for(y=0;y<=(selection_end_row - selection_start_row);y++) {
    if(Cursor.row+y > 0x0F) break;
    
    for(x=0;x<=(selection_end_col-selection_start_col);x++) {
      if((Cursor.col+x) < 7) {
        *cell++ = *cp++;
      } else {
        cp+=((selection_end_col-selection_start_col)-x)+1;
        break;
      }
    }
    cell+=(7-x);
  }

  /* Move & clip cursor */
  uint16_t row = (Cursor.row+y);
  if(row > 0x0F) {
    row = 0x0F;
  } else {
    row = (Cursor.row+y);
  }
  if(row > 0xFF) row = 0xFF;

  Cursor.row = row;
  setData();
  Cursor.set(Cursor.col,Cursor.row);
  setCell();
}

void PatternClass::actionCreate(uint8_t input)
{
  if(*cell == 0xFF) {
    if(Cursor.col == 0) {
      for(uint8_t i=0;i!=3;i++) {
        *(cell+i) = lastEdit[i];
      }
      Display.setData(cell, 3, Cursor.col, Cursor.row);
    } else {
      *(cell) = lastEdit[Cursor.col];
      Display.setData(cell, 1, Cursor.col, Cursor.row);
    }
  } else {
    actionEdit(input);
  }
}

void PatternClass::actionEdit(uint8_t input)
{
  
  int8_t horz_move = input&J_LEFT ? -1 : input&J_RIGHT ? 1 : 0;
  int8_t vert_move = 0;
  if(Cursor.col == 0) {
    vert_move = input&J_UP ? 12 : input&J_DOWN ? -12 : 0;
  } else {
    vert_move = input&J_UP ? 16 : input&J_DOWN ? -16 : 0;
  }

  *cell += horz_move;
  *cell += vert_move;

  if(*cell > 0xE0) {
    *cell = 0;
  }

  if(*cell > 0x7F) {
    *cell = 0x7F;
  }

  lastEdit[Cursor.col] = *cell;

  Display.setData(cell, 1, Cursor.col, Cursor.row);

}

void PatternClass::actionDelete(uint8_t input)
{
  *cell = 0xFF;

  if(Cursor.col == 0) {
    *(cell+1) = 0xFF;
    *(cell+2) = 0xFF;
    Display.setData(cell, 3, Cursor.col, Cursor.row);
  } else {
    Display.setData(cell, 1, Cursor.col, Cursor.row);
  }
}

void PatternClass::actionMove(uint8_t input)
{
  
  int8_t horz_move = input&J_LEFT ? -1 : input&J_RIGHT ? 1 : 0;
  int8_t vert_move = input&J_UP ? -1 : input&J_DOWN ? 1 : 0;

  Cursor.move(horz_move, vert_move);
  setCell();
  if(selection_active) {
    Cursor.endSelection();
  }
}

void PatternClass::setData()
{
  Display.setData(&memory.pattern[view_pattern][0][0], PATTERN_COLUMNS * ROW_LENGTH);
}

void PatternClass::setCell()
{
  cell = &memory.pattern[view_pattern][Cursor.row][Cursor.col];
}

void PatternClass::setView(uint8_t pattern)
{
  if(pattern < PATTERN_SIZE) view_pattern = pattern;
}

PatternClass Pattern;