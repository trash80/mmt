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

#include "display.h"
#include "cursor.h"
#include "joypad.h"
#include "player.h"
#include "chain.h"
#include "song.h"

SongClass::SongClass()
{
  view_offset = 0;
}

void SongClass::begin()
{

}

void SongClass::init()
{
  Display.setView(0);
  Cursor.set(0,0);
  setData();
  setCell();
}

void SongClass::update()
{
  
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
  }
}

void SongClass::actionCue(uint8_t input)
{
  Display.setSprite(0,62,5,4,100);
  Display.setSprite(1,62,5+3,4,100);
  Display.setSprite(2,62,5+6,4,100);
}

void SongClass::actionSelect(uint8_t input)
{
  Cursor.startSelection();
  selection_start = Cursor.pos;
  selection_active = true;
}

void SongClass::actionCopy(uint8_t input)
{
  if(!selection_active) return;
  
  Cursor.clearSelection();
  Cursor.set(Cursor.col,Cursor.row);

  uint8_t * cb = &m_copy_buffer[0];
  uint8_t * mp = &m_song[0][0];
  mp += selection_start;
  selection_size = selection_end - selection_start;
  for(int x=0;x!=selection_size;x++) {
    *cb++ = *mp++;
  }
  setCell();
  selection_active = false;
}

void SongClass::actionPaste(uint8_t input)
{
  uint8_t * cb = &m_copy_buffer[0];

  for(int x=0;x!=selection_size;x++) {
    *cell++ = *cb++;
  }
  setData();
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
    setData();
    vert_move = 0;
  } else if(vert_move == 1 && Cursor.row == 0x0F && (view_offset+16) < SONG_SIZE) {
    view_offset+=vert_move;
    setData();
  } else if (vert_move == -1 && Cursor.row == 0x00 && view_offset) {
    view_offset+=vert_move;
    setData();
  }

  Cursor.move(horz_move, vert_move);
  setCell();
  if(selection_active) {
    selection_end = Cursor.pos < selection_start ? selection_start : Cursor.pos;
    selection_start = Cursor.pos < selection_start ? Cursor.pos : selection_start;
    Cursor.endSelection();
  }
}

void SongClass::setData()
{
  char * s = &Display.screen[(40*3)+2];
  for(uint16_t y=view_offset;y<16+view_offset;y++) {
    Display.printHex(s,y);
    s+=40;
  }
  Display.setData(&m_song[view_offset][0], 128);
}

void SongClass::setCell()
{
  cell = &m_song[Cursor.row + view_offset][Cursor.col];
  Chain.setView(Cursor.col,*cell);
}

SongClass Song;