#include "display.h"
#include "cursor.h"

CursorClass::CursorClass() {
  
}

void CursorClass::begin() {

}

void CursorClass::draw() {

  mask * m = &masks[0];
  mask * ml = &masks[1];
  if(m->active) {
    if(ml->active) {
      Display.fillArea(ml->x,ml->y,ml->width,ml->height,ml->color);
      Display.setRedrawRegion(ml->x,ml->y,ml->width,ml->height);
      ml->active = false;
    }
    ml->x = m->x;
    ml->y = m->y;
    ml->color = 0;
    ml->width = m->width;
    ml->height = m->height;
    ml->active = true;
    Display.fillArea(m->x,m->y,m->width,m->height,m->color);
    Display.setRedrawRegion(m->x,m->y,m->width,m->height);
    m->active = false;
  }
}

void CursorClass::set(uint8_t new_col, uint8_t new_row)
{
  uint16_t new_pos = Display.getDataPosition(new_col,new_row);
  if (new_pos == 0) return;
  
  uint8_t s = Display.getDataPositionSize(new_pos);

  if(abs_pos && size) {
    //reset old position's color
    char * c = &Display.color[abs_pos];
    const char * rc = Display.getColorPointerAtPosition(abs_pos);
    while(size--) {
      *c++ = *rc++;
    }
  }

  col = new_col;
  row = new_row;
  pos = col+(row*40);
  abs_pos = new_pos;
  real_col = abs_pos%40;
  real_row = abs_pos/40;
  size= s;

  if(!selection_active) {
    char * c = &Display.color[abs_pos];
    while(s--) {
      //58 is ascii char == position 10
      (*c++) = 58;
    }

    mask * m = &masks[0];
    m->x = real_col;
    m->y = real_row;
    m->width = size;
    m->height = 1;
    m->color = rgb(255,0,210);
    m->active = true;
  }
}

void CursorClass::clear()
{
  mask * m = &masks[0];
  m->x = real_col;
  m->y = real_row;
  m->width = size;
  m->height = 1;
  m->color = 0;
  m->active = true;

  col = 0;
  row = 0;
  pos = 0;
  size= 1;
  abs_pos = 0;
}

void CursorClass::move(int8_t c, int8_t r)
{
  c += col;
  r += row;

  if (c < 0) return;
  if (r < 0) return;
  set(c,r);
}

void CursorClass::startSelection()
{
  drawSelection(0);
  selection_start_pos = abs_pos;
  selection_start_col = (real_col);
  selection_start_row = (real_row);
  selection_start_size = size;

  selection_end_pos   = abs_pos;
  selection_end_col   = (real_col + size);
  selection_end_row   = (real_row);
  selection_active = true;
  drawSelection(1);
}

void CursorClass::endSelection()
{
  drawSelection(0);
  selection_end_pos   = abs_pos;
  selection_end_col   = real_col + size;
  selection_end_row   = real_row;
  drawSelection(1);
}

void CursorClass::clearSelection()
{
  drawSelection(0);
  selection_active = false;
  selection_start_row = selection_end_row = selection_start_col = selection_end_col = 0;
}

void CursorClass::drawSelection(bool show)
{
  uint16_t p;
  uint8_t row_start = selection_start_row;
  uint8_t row_end   = selection_end_row;
  uint8_t col_start = selection_start_col;
  uint8_t col_end   = selection_end_col;

  uint16_t c = show == 0 ? 0 : Display.getPalletColor(11,1);

  if(selection_start_row > selection_end_row) {
    row_start = selection_end_row;
    row_end   = selection_start_row;
  }

  if(selection_start_col > selection_end_col) {
    col_start = selection_end_col - size;
    col_end   = selection_start_col + selection_start_size;
  }
  
  // @TODO performance improvements
  for(uint8_t h=row_start;h<row_end + 1;h++) {
    p = col_start+(h*40);
    if(show) {
      char *c =& Display.color[p];
      for(uint8_t w=col_start;w<col_end;w++) {
        //59 is ascii char for ; == position 11
        (*c++) = 59;
      }
    } else {
      char *c =& Display.color[p];

      const char *cp = Display.getColorPointerAtPosition(p);
      for(uint8_t w=col_start;w<col_end;w++) {
        (*c++) = (*cp++);
      }
    }
  }
  mask * m = &masks[0];
  m->x = col_start;
  m->y = row_start;
  m->width = col_end - col_start;
  m->height = row_end - row_start + 1;
  m->color = c;
  m->active = true;

}

CursorClass Cursor;
