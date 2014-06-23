/*
 * Display class File - Handles all things display related
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
#include "font.c"
#include "screens.c"

DisplayClass::DisplayClass() : TFT_ILI9340(_cs, _dc, _rst) {
  
}

void DisplayClass::enable() {
  _enable = true;
}

void DisplayClass::disable() {
  _enable = false;
}
    
void DisplayClass::begin() {
  TFT_ILI9340::begin();
  TFT_ILI9340::setBitrate(24000000);
  
  setRotation(1);
  fillScreen(rgb(0,0,0));
  writeBuffer= true;
  _enable = true;
}


void DisplayClass::splash() {
  
  setTextSize(4);
  print("n o n > t r > c k");
  fillScreen(rgb(0,0,0));
  print("n o n > t r > c k");
  delay(10);
  fillScreen(rgb(0,0,255));
  fillScreen(rgb(255,0,0));
  print("n o n > t r > c k");
  delay(10);
  fillScreen(rgb(255,0,0));
  fillScreen(rgb(0,0,255));
  print("n o n > t r > c k");
  delay(10);
  fillScreen(rgb(255,0,0));
  fillScreen(rgb(0,255,0));
  fillScreen(rgb(0,0,255));
  print("n o n > t r > c k");
  delay(10);
  fillScreen(rgb(0,0,0));
  setCursorAtPixel(5,100);
  setTextColor(rgb(255,255,255));
  print("n o n > t r > c k");
  delay(500);
  setCursorAtPixel(5,130);
  setTextSize(2);
  setTextColor(rgb(100,100,100));
  print("by trash80");
  setCursorAtPixel(5,150);
  print("v.0.0.1");
  delay(1000);
}

void DisplayClass::draw() {
  if(!_enable || time > millis()) return ;

  time = millis() + 33;
  frame++;


  Cursor.draw(); 
  enableBuffer = true;
  writeBuffer  = true;
  setCursorAtPixel(0,0);
  cPrint(screen,color);
  enableBuffer = false;
  drawSprites();
}

void DisplayClass::setView(uint8_t sc) {
  Cursor.clear();
  scene = sc;
  memcpy(screen, screens[sc], 801);
  memcpy(color, colors[colorMap[sc]], 801);
  
  Cursor.clearSelection();
  setTextSize(2);
  setCursorAtPixel(0,0);
  
}

void DisplayClass::setRedrawRegion(uint8_t col, uint8_t row, uint8_t width, uint8_t height)
{
  uint16_t pos;
  for(uint8_t h=row;h<(row+height);h++) {
    pos = (col)+(h*40);
    char *c =& buffer_char[pos];
    uint16_t *s =& buffer_color[pos];

    for(uint8_t w=col;w<(col+width);w++) {
      (*c++) = -1;
      (*s++) = -1;
    }
  }
}

void DisplayClass::redrawWindow(uint8_t col, uint8_t row, uint8_t width, uint8_t height)
{
  uint16_t pos;
  
  for(uint8_t h=row;h<(row+height);h++) {
    pos = (col)+(h*40);
    char *c =& color[pos];
    char *s =& screen[pos];
    
    setPosition(col,h);
    
    for(uint8_t w=col;w<(col+width);w++) {
      const unsigned int *p =& pallet[((*c++) - 48)][0];
      textcolor   = *p++;
      textbgcolor = *p;
      print((*s++));
    }
  }
}

void DisplayClass::setData(uint8_t * data, uint16_t size, uint8_t col, uint8_t row)
{
  uint16_t pos, last_pos = 0;
  const char * sc =& screens[scene][0];
  char *s =& screen[0];

  pos = getDataPosition(col, row);
  while(size-- && pos) {

    sc += (pos - last_pos);
    s  += (pos - last_pos);
    switch((*sc)) {
      case 99: // "c" - CHAR
        (*s) = (*data);
        break;
      case 104: // "h" - HEX
        printHex(s,*data);
        break;
      case 120: // "x" - HEX under 128
        if(!((*data) & 0x80)) {
          printHex(s,*data);
        } else {
          //clear
          *s = 126;
          *(s+1) = 126;
        }
        break;
      case 105: // "i" - INTEGER
        sprintf(s,"%u",*data);
        if(!*(s+1)) *(s+1) = (*sc+1);
        if(!*(s+2)) *(s+2) = (*sc+2);
        if(!*(s+3)) *(s+3) = (*sc+3);
        break;
      case 110: // "n" - NOTE
        printNote(s,*data);
        break;
    }
    data++;
    last_pos = pos;
    pos = getNextDataPosition(pos);
  }
  
  enableBuffer = true;
  redrawWindow(0,0,40,20);
  enableBuffer = false;
}

void DisplayClass::setCursorAtPixel(int16_t x, int16_t y) {
  cursor_x = x;
  cursor_y = y;
}

void DisplayClass::fillArea(uint8_t col, uint8_t row, uint8_t width, uint8_t height, uint16_t c)
{
  fillRect(((col)*8)+1, (row*12)-1, (width*8), (height*12), c);
}

void DisplayClass::setSprite(uint8_t sprint_index, const char chr, uint8_t col, uint8_t row)
{
  setSprite(sprint_index, chr, col, row, 0, pallet[9][0]);
}

void DisplayClass::setSprite(uint8_t sprint_index, const char chr, uint8_t col, uint8_t row, uint16_t lifetime)
{
  setSprite(sprint_index, chr, col, row, lifetime, pallet[9][0]);
}

void DisplayClass::setSprite(uint8_t sprint_index, const char chr, uint8_t col, uint8_t row, uint16_t lifetime, uint16_t color)
{
  struct sprite * sp =& sprites[sprint_index];
  (*sp).chr = chr;
  (*sp).pos_x = col;
  (*sp).pos_y = row;
  (*sp).color = color;
  (*sp).lifetime = lifetime;
  (*sp).active = true;
}

void DisplayClass::clearSprites()
{
  struct sprite * sp =& sprites[0];
  
  for(uint8_t x=0;x!=16;x++) {
    redrawWindow((*sp).pos_x_last,(*sp).pos_y_last, 1, 1);
    redrawWindow((*sp).pos_x,(*sp).pos_y, 1, 1);
    (*sp).pos_x_last = (*sp).pos_x = 0xFF;
    (*sp).pos_y_last = (*sp).pos_y = 0xFF;
    (*sp).active = false;
  }
}

void DisplayClass::drawSprites()
{
  struct sprite * sp =& sprites[0];
  writeBuffer = false;
  for(uint8_t x=0;x!=16;x++) {
    if((*sp).active && (*sp).pos_x < 40 && (*sp).pos_y < 20) {
      if((*sp).lifetime == 1) {
          (*sp).active = false;
          redrawWindow((*sp).pos_x,(*sp).pos_y, 1, 1);
          continue;
      } else if ((*sp).lifetime) {
          (*sp).lifetime--;
      }

      if ((*sp).pos_x_last != 0xFF && (*sp).pos_y_last != 0xFF 
        && (*sp).pos_x_last != (*sp).pos_x && (*sp).pos_y_last != (*sp).pos_y) {
        redrawWindow((*sp).pos_x_last,(*sp).pos_y_last, 1, 1);
      }
      setTextColor((*sp).color, (*sp).color);
      setPosition((*sp).pos_x,(*sp).pos_y);
      print((*sp).chr);
      (*sp).pos_x_last = (*sp).pos_x;
      (*sp).pos_y_last = (*sp).pos_y;

    } else if ((*sp).pos_x != 0xFF && (*sp).pos_y != 0xFF) {
      redrawWindow((*sp).pos_x,(*sp).pos_y, 1, 1);
      (*sp).pos_x = 0xFF;
      (*sp).pos_y = 0xFF;
    } else if ((*sp).pos_x_last != 0xFF && (*sp).pos_y_last != 0xFF) {
      redrawWindow((*sp).pos_x_last,(*sp).pos_y_last, 1, 1);
      (*sp).pos_x_last = 0xFF;
      (*sp).pos_y_last = 0xFF;
    }
    sp++;
  }
  writeBuffer = true;
  
}

void DisplayClass::setPosition(uint8_t col, uint8_t row)
{
  setCursorAtPixel(col*8, row*12);
}

uint16_t DisplayClass::getDataPosition(uint8_t find_col, uint8_t find_row)
{
  uint8_t c = 0;
  uint8_t r = 255;
  uint16_t pos = 0;

  const char * sc =& screens[scene][0];
  
  //Find the row
  while((*sc) && (r != find_row)) {
    if((*sc) == 123) { // 123 == "{"
      r++;
    }
    sc++;
    pos++;
    if(!(*sc)) return 0;
  }
  if(r != find_row) return 0;
  sc++;

  //Find the column
  while((*sc) && (c != find_col)) {
    while((*sc) < 97 || (*sc) == 126) { // 126 == "~"
      sc++;
      pos++;
    }
    c++;
    if((*sc) == 125) { // 125 == "}"
      //End of col return
      return 0;
    }
    sc++;
    pos++;

    if(!(*sc)) return 0;
  }
  if(find_col != c) return 0;
  return pos;
}

uint8_t DisplayClass::getDataPositionSize(uint16_t pos)
{
  const char * sc =& screens[scene][pos + 1];
  uint8_t size = 1;

  while((*sc) && (*sc) != 32 && (((*sc) > 64 && (*sc) < 97) || (*sc) == 126)) {
    size++;
    sc++;
  }

  return size;
}

uint16_t DisplayClass::getNextDataPosition(uint16_t pos)
{

  const char * sc =& screens[scene][pos + 1];
  
  //Find the column
  while((*sc)) {
    if((*sc) == 125) { // hit a }
      while((*sc) && (*sc) != 123) { //find next {
        sc++;
        pos++;
      }
    }
    if((*sc) > 96 && (*sc) < 123) { // a to z
      return pos+1;
    }
    sc++;
    pos++;
  }
  return 0;
}

const char * DisplayClass::getColorPointerAtPosition(uint16_t pos)
{
    return &colors[colorMap[scene]][pos];
}

uint16_t DisplayClass::getPalletColor(uint8_t section, uint8_t color)
{
    return pallet[section][color];
}

void DisplayClass::printHex(char * buffer, uint8_t value)
{
  uint8_t nibble = value>>4;

  if(!nibble) {
    *(buffer++) = 48; // ascii chr 0 
  } else {
    if(nibble > 0x09) {
      *(buffer++) = 65+(nibble - 10); // ascii chr A
    } else {
      *(buffer++) = 48+nibble; // ascii chr 0 
    }
  }
  nibble = value&0x0F;
  if(nibble > 0x09) {
    *(buffer) = 65+(nibble-10);
  } else {
    *(buffer++) = 48+nibble; // ascii chr 0 
  }
}

void DisplayClass::printNote(char * buffer, uint8_t note)
{
  const char * name = &noteNames[(note%12)*2];
  *buffer = *name;
  *(buffer+1) = *(name+1);
  *(buffer+2) = (note/12)+48;
}


void DisplayClass::printBuffer(uint8_t col, uint8_t row, const char *str) {
  printBuffer(col,row,0,&(*str));
}

void DisplayClass::printBuffer(uint8_t col, uint8_t row, uint8_t col_size, const char *str) {
  uint16_t pos = col + (row*40);
  uint8_t col_count = 0;
  char *s =& screen[pos];
  char *c =& color[pos];
  
  setPosition(col,row);
  while((*str)) {
    if(col_size && col_count == col_size) {
        col_count = 0;
        row++;
        setPosition(col,row);
        pos = col + (row*40);
        s =& screen[pos];
        c =& color[pos];
    }
    col_count++;
    
    const unsigned int *p =& pallet[((*c++) - 48)][0];
    textcolor   = *p++;
    textbgcolor = *p;
    print(*str);
    *(s++) = (*str++);
  }
}

size_t DisplayClass::cPrint(const char *str, const char *color) {
  if (str == NULL) return 0;
  return write((const uint8_t *)str, (const uint8_t *)color, strlen(str));
}

size_t DisplayClass::write(const uint8_t *buffer, const uint8_t *color, size_t size)
{
  size_t n = 0;
  while (size--) {
    textcolor   = pallet[(*color) - 48][0];
    textbgcolor = pallet[(*color++) - 48][1];
    n += write(*buffer++);
  }
  return n;
}

/* Overwritten from Adafruit_GFX for new font size */
size_t DisplayClass::write(uint8_t c) {
  if (c == '\n') {
    cursor_y += textsize*6;
    cursor_x  = 0;
  } else if (c == '\r') {
    // skip em
  } else {
    drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
    cursor_x += textsize*4;
    if (wrap && (cursor_x > (_width - textsize*4))) {
      cursor_y += textsize*6;
      cursor_x = 0;
    }
  }
  return 1;
}

/* Overwritten from Adafruit_GFX for new font size */
void DisplayClass::drawChar(int16_t x, int16_t y, unsigned char c,
			    uint16_t color, uint16_t bg, uint8_t size) {

  if((x >= _width)            || // Clip right
     (y >= _height)           || // Clip bottom
     ((x + 4 * size - 1) < 0) || // Clip left
     ((y + 6 * size - 1) < 0))   // Clip top
    return;

  uint16_t pos = (x / ((3+1)*size)) + ( (y / ((5+1)*size)) * 40 );

  if(c == 123 || c == 125) c = 32;
  if(c > 90) c = 126;
  //Special dashes "~"
  if(c == 126) color = pallet[12][0];
  
  
  if(!enableBuffer || buffer_char[pos] != c || buffer_color[pos] != color) {
    if(writeBuffer) {
      buffer_char[pos] = c;
      buffer_color[pos]= color;
    }
    if(c == 0 || c == 32) {
      if(bg == color) return;
      fillRect(x, y, 4*size, 5*size, bg);
    } else {
      for (int8_t i=0; i!=5; i++ ) {
        char line = *(font+((c-32)*5)+i);
        for (int8_t j = 0; j!=3; j++) {
          if (line & 0x1) {
            if (size == 1) // default size
              drawPixel((x+3)-j, y+i, color);
            else {  // big size
              fillRect((x+(3*size))-(j*size), y+(i*size), size, size, color);
            } 
          } else if (bg != color) {
            if (size == 1) // default size
              drawPixel((x+3)-j, y+i, bg);
            else {  // big size
              fillRect((x+(3*size))-(j*size), y+(i*size), size, size, bg);
            }
          }
          line >>= 1;
        }
      }
    }
  }
}


DisplayClass Display;
