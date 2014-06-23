#include "display.h"
#include "cursor.h"
#include "joypad.h"
#include "player.h"
#include "chain.h"

ChainClass::ChainClass()
{
  view_offset = 0;
}

void ChainClass::begin()
{

}

void ChainClass::init()
{
  Display.setView(3);
  Cursor.set(0,0);
  setData();
  setCell();
}

void ChainClass::update()
{
  
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
  }
}

void ChainClass::actionCue(uint8_t input)
{
  /*
  Display.setSprite(0,62,5,4,100);
  Display.setSprite(1,62,5+3,4,100);
  Display.setSprite(2,62,5+6,4,100);
  */
}

void ChainClass::actionSelect(uint8_t input)
{
  /*
  Cursor.startSelection();
  selectionMode = true;
  */
}

void ChainClass::actionCopy(uint8_t input)
{
  /*
  Cursor.clearSelection();
  Cursor.set(Cursor.col,Cursor.row);
  setCell();
  selectionMode = false;
  */
}

void ChainClass::actionPaste(uint8_t input)
{

}

void ChainClass::actionCreate(uint8_t input)
{
  /*
  if(*cell == 0xFF) {
    *cell = 0x00;
  }
  actionEdit(input);
  */
}

void ChainClass::actionEdit(uint8_t input)
{
  /*
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
  */

}

void ChainClass::actionDelete(uint8_t input)
{
  /*
  *cell = 0xFF;
  Display.setData(cell, 1, Cursor.col, Cursor.row);
  */
}

void ChainClass::actionMove(uint8_t input)
{
  /*
  int8_t horz_move = input&J_LEFT ? -1 : input&J_RIGHT ? 1 : 0;
  int8_t vert_move = input&J_UP ? -1 : input&J_DOWN ? 1 : 0;

  if(vert_move == 1 && Cursor.row == 0x0F && (view_offset+16) < SONG_SIZE) {
    view_offset++;
    setData();
  } else if (vert_move == -1 && Cursor.row == 0x00 && view_offset) {
    view_offset--;
    setData();
  }

  Cursor.move(horz_move, vert_move);
  setCell();
  if(selectionMode) {
    Cursor.endSelection();
  }
  */
}

void ChainClass::setData()
{
  /*
  char * s = &Display.screen[(40*3)+2];
  for(uint16_t y=view_offset;y<16+view_offset;y++) {
    Display.printHex(s,y);
    s+=40;
  }
  Display.setData(&m_chain[view_offset][0], 128);
  */
}

void ChainClass::setCell()
{
  //cell = &m_chain[Cursor.row + view_offset][Cursor.col];
}

void ChainClass::setView(uint8_t track, uint8_t chain)
{
  
}

ChainClass Chain;