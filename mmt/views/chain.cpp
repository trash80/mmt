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

#include "chain.h"

ChainClass::ChainClass()
{
  view_chain = 0;
  view_row   = 0;
  view_col   = 0;
}

void ChainClass::begin()
{

}

void ChainClass::init()
{
  Display.clearSprites();
  Display.setView(CHAIN_VIEW);

  // Set the chain # 
  Display.printHex(&Display.screen[8],view_chain);

  setData();
  setCell();
  Cursor.set(view_col,view_row);
}

void ChainClass::update()
{

}

void ChainClass::draw()
{
  Display.clearSprites();
  if(!Player.playing()) return ;

  if(Player.isChainMode()) {
    if(Player.tracks[0].play_position == view_chain) {
      Display.setSprite(0,62,4,3+(Player.tracks[0].chain_pos));
    }
  } else if(Player.isSongMode() && (*Player.tracks[Song.track()].song) == view_chain) {
    Display.setSprite(0,62,4,3+(Player.tracks[Song.track()].chain_pos));
  }
}

void ChainClass::action(uint8_t input)
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
      Route.setView(SONG_VIEW);
      break;
    case J_SELECT|J_RIGHT:
      Pattern.setView(*cell);
      Route.setView(PATTERN_VIEW);
      break;
  }
}

void ChainClass::actionCue(uint8_t input)
{
  if(!Player.playing() || (Player.playing() && !Player.isChainMode())) {
    Player.playChain(view_chain,Cursor.row);
  } else {
    Player.stop();
  }
}

void ChainClass::actionSelect(uint8_t input)
{
  
  Cursor.startSelection();
  selection_start = Cursor.pos;
  selection_active = true;
  
}

void ChainClass::actionCopy(uint8_t input)
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
  volatile uint8_t * sp = &memory.chain[view_chain][selection_start_row][selection_start_col];

  uint8_t mem_count = 0;
  for(uint16_t y=selection_start_row;y<=selection_end_row;y++) {
    for(uint8_t x=selection_start_col;x<=selection_end_col;x++) {
      if(mem_count >= 256) break;
      *cp++ = *sp++;
      mem_count++;
    }
    if(y != 16) sp+=(1-(selection_end_col-selection_start_col));
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

void ChainClass::actionPaste(uint8_t input)
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
      if((Cursor.col+x) < 2) {
        *cell++ = *cp++;
      } else {
        cp+=((selection_end_col-selection_start_col)-x)+1;
        break;
      }
    }
    cell+=(2-x);
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

void ChainClass::actionCreate(uint8_t input)
{
  if(*cell == 0xFF) {
  //  *cell = 0x00;
  }
  actionEdit(input);
}

void ChainClass::actionEdit(uint8_t input)
{
  
  int8_t horz_move = input&J_LEFT ? -1 : input&J_RIGHT ? 1 : 0;
  int8_t vert_move = 0;
  if(Cursor.col == 1) {
    vert_move = input&J_UP ? 12 : input&J_DOWN ? -12 : 0;
  } else {
    vert_move = input&J_UP ? 16 : input&J_DOWN ? -16 : 0;
  }

  *cell += horz_move;
  *cell += vert_move;
  if(Cursor.col == 0) {
    //Phrase column limits
    if(*cell > 0xE0) {
      *cell = 0x00;
    }
    if(*cell > 0x7F) {
      *cell = 0x7F;
    }
  }

  Display.setData(cell, 1, Cursor.col, Cursor.row);

}

void ChainClass::actionDelete(uint8_t input)
{
  *cell = Cursor.col == 0 ? 0xFF : 0x00;
  Display.setData(cell, 1, Cursor.col, Cursor.row);
}

void ChainClass::actionMove(uint8_t input)
{
  
  int8_t horz_move = input&J_LEFT ? -1 : input&J_RIGHT ? 1 : 0;
  int8_t vert_move = input&J_UP ? -1 : input&J_DOWN ? 1 : 0;

  Cursor.move(horz_move, vert_move);
  setCell();
  if(selection_active) {
    Cursor.endSelection();
  }
}

void ChainClass::setData()
{
  Display.setData(&memory.chain[view_chain][0][0], 32);
}

void ChainClass::setCell()
{
  view_row = Cursor.row;
  view_col = Cursor.col;
  cell = &memory.chain[view_chain][Cursor.row][Cursor.col];
}

void ChainClass::setView(uint8_t chain)
{
  if(chain > -1 && chain < CHAIN_SIZE) view_chain = chain;
}

ChainClass Chain;