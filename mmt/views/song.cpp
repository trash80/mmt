/*
 * Song Controller class File
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

#include "song.h"

SongClass::SongClass()
{
  view_offset = 0;
  view_track  = 0;
  view_row    = 0;
}

void SongClass::begin()
{

}

void SongClass::init()
{
  Display.clearSprites();
  Display.setView(SONG_VIEW);
  Cursor.set(view_track,view_row);
  setData();
  setCell();
}

void SongClass::update()
{

}

void SongClass::draw()
{
  Display.clearSprites();
  if(Player.playing() && Player.isSongMode()) {
    for(uint8_t t=0;t!=NUM_TRACKS;t++) {
      //uint8_t s = Player.tracks[t].step_pos;
      //s %=2;

      if(!Player.tracks[t].playing) {
        Display.clearSprite(t);
      } else if (Player.tracks[t].song_pos >= view_offset) {
        Display.setSprite(t,62,5+(t*3),3+(Player.tracks[t].song_pos - view_offset));
      }
    }
  }
}

void SongClass::action(uint8_t input)
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
    case J_UP|J_B:
    case J_DOWN|J_B:
      actionMove(input);
      break;
    case J_SELECT|J_UP:
      break;
    case J_SELECT|J_DOWN:
      break;
    case J_SELECT|J_LEFT:
      break;
    case J_SELECT|J_RIGHT:
      Chain.setView(*cell);
      Route.setView(CHAIN_VIEW);
      break;
  }
}

void SongClass::actionCue(uint8_t input)
{
  if(!Player.playing() || (Player.playing() && !Player.isSongMode())) {
    Player.playSong(Cursor.row + view_offset);
  } else {
    Player.stop();
  }
}

void SongClass::actionSelect(uint8_t input)
{
  Cursor.startSelection();
  selection_start = Cursor.pos;
  selection_active = true;
}

void SongClass::actionCopy(uint8_t input)
{
  /* @TODO Cleanup / refactor */
  if(!selection_active) return;
  uint8_t selection_start_col,selection_end_col,selection_start_row,selection_end_row;

  selection_end = Cursor.pos < selection_start ? selection_start : Cursor.pos;
  selection_start = Cursor.pos < selection_start ? Cursor.pos : selection_start;
  
  selection_start_col = (selection_start%8);
  selection_start_row = (selection_start/40) + view_offset;
  selection_end_col = (selection_end%8);
  selection_end_row = (selection_end/40) + view_offset;
  if(selection_start_col > selection_end_col) {
    selection_start_col = selection_end_col;
    selection_end_col = (selection_start%8);
  }

  volatile uint8_t * cp = &memory.copy_buffer[0];
  volatile uint8_t * sp = &memory.song[selection_start_row][selection_start_col];

  uint8_t mem_count = 0;
  for(uint16_t y=selection_start_row;y<=selection_end_row;y++) {
    for(uint8_t x=selection_start_col;x<=selection_end_col;x++) {
      if(mem_count >= 256) break;
      *cp++ = *sp++;
      mem_count++;
    }
    if(y != 255) sp+=(7-(selection_end_col-selection_start_col));
  }

  /* Move & clip cursor */
  Cursor.clearSelection();
  Cursor.col = selection_start_col;
  Cursor.row = (((uint16_t)selection_end_row) - view_offset +1)&255;
  if(Cursor.row > 0x0F) {
    Cursor.row = 0x0F;
    if(view_offset < 0xF0) view_offset++;
    setData();
  }
  Cursor.set(Cursor.col,Cursor.row);
  setCell();
  selection_active = false;
}

void SongClass::actionPaste(uint8_t input)
{
  /* @TODO Cleanup / refactor */
  uint8_t selection_start_col,selection_end_col,selection_start_row,selection_end_row;

  selection_start_col = (selection_start%8);
  selection_start_row = (selection_start/40) + view_offset;
  selection_end_col = (selection_end%8);
  selection_end_row = (selection_end/40) + view_offset;
  if(selection_start_col > selection_end_col) {
    selection_start_col = selection_end_col;
    selection_end_col = (selection_start%8);
  }

  volatile uint8_t * cp = &memory.copy_buffer[0];
  uint8_t x,y;
  for(y=0;y<=(selection_end_row - selection_start_row);y++) {
    if(Cursor.row+y > 0xFF) break;
    
    for(x=0;x<=(selection_end_col-selection_start_col);x++) {
      if((Cursor.col+x) < 8) {
        *cell++ = *cp++;
      } else {
        cp+=((selection_end_col-selection_start_col)-x)+1;
        break;
      }
    }
    cell+=(8-x);
  }

  /* Move & clip cursor */
  uint16_t row = (Cursor.row+y)+view_offset;
  if(row > view_offset+0x0F) {
    view_offset = row-0x0F < 0xF0 ? row-0x0F : 0xF0;
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

void SongClass::actionCreate(uint8_t input)
{
  if(*cell == 0xFF) {
    *cell = 0x00;
  }
  actionEdit(input);
}

void SongClass::actionEdit(uint8_t input)
{
  int8_t horz_move = input&J_LEFT ? -1 : input&J_RIGHT ? 1 : 0;
  int8_t vert_move = input&J_UP ? 16 : input&J_DOWN ? -16 : 0;

  *cell += horz_move;
  *cell += vert_move;
  if(*cell > 0xE0) {
    *cell = 0x00;
  }
  if(*cell > 0x7F) {
    *cell = 0x7F;
  }
  Display.setData(cell, 1, Cursor.col, Cursor.row);
  setCell();

}

void SongClass::actionDelete(uint8_t input)
{
  *cell = 0xFF;
  Display.setData(cell, 1, Cursor.col, Cursor.row);
}

void SongClass::actionMove(uint8_t input)
{
  int16_t horz_move = input&J_LEFT ? -1 : input&J_RIGHT ? 1 : 0;
  int16_t vert_move = input&J_UP ? -1 : input&J_DOWN ? 1 : 0;
  if(input&J_B && vert_move) {
    vert_move *= 16;
    vert_move += view_offset;
    if(vert_move >= 240) {
      view_offset = 240;
    } else if (vert_move < 0) {
      view_offset = 0;
    } else {
      view_offset = vert_move;
    }
    Cursor.move(horz_move, vert_move);
    setData();
    vert_move = 0;
  } else if(vert_move == 1 && Cursor.row == 0x0F && (view_offset+16) < SONG_SIZE) {
    Cursor.move(horz_move, vert_move);
    Cursor.moveSelection(0,-1);
    view_offset+=vert_move;
    setData();
  } else if (vert_move == -1 && Cursor.row == 0x00 && view_offset) {
    view_offset+=vert_move;
    Cursor.move(horz_move, vert_move);
    Cursor.moveSelection(0,1);
    setData();
  } else {
    Cursor.move(horz_move, vert_move);
    Cursor.endSelection();
  }

  setCell();
}

void SongClass::setData()
{
  char * s = &Display.screen[(40*3)+2];
  for(uint16_t y=view_offset;y<16+view_offset;y++) {
    Display.printHex(s,y);
    s+=40;
  }
  Display.setData(&memory.song[view_offset][0], 128);
}

void SongClass::setCell()
{
  view_track = Cursor.col;
  view_row   = Cursor.row;
  cell = &memory.song[Cursor.row + view_offset][Cursor.col];
}

SongClass Song;