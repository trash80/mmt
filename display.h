#ifndef display_h
#define display_h

#include "Arduino.h"
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "TFT_ILI9340.h"

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

#define _cs 10
#define _dc 9
#define _rst 14

#ifndef rgb
#define rgb(r, g, b) (unsigned int) ((r / 8) << 11) | ((g / 4) << 5) | (b / 8)
#endif

class DisplayClass : public TFT_ILI9340 {
  public:
    DisplayClass();
    void begin();
    void draw();
    void enable();
    void disable();
    
    void setView(uint8_t sc);
    
    void setSprite(uint8_t sp, char, uint8_t col, uint8_t row);
    void setSprite(uint8_t sp, char, uint8_t col, uint8_t row, uint16_t lifetime);
    void setSprite(uint8_t sp, char, uint8_t col, uint8_t row, uint16_t lifetime, uint16_t color);
    void moveSprite(uint8_t sp, uint8_t col, uint8_t row);
    void hideSprite(uint8_t sp);
    void showSprite(uint8_t sp);
    void drawSprites();
    void clearSprites();
    
    void setScroll(char *str);
    void clearScroll();
    
    void setData(uint8_t * data, uint16_t size, uint8_t col, uint8_t row);
    void setData(uint8_t * data, uint16_t size) { setData(data, size, 0, 0); }
    void printNote(char * buffer,uint8_t value);
    void printHex(char * buffer,uint8_t value);
    void setCursorAtPixel(int16_t x, int16_t y);
    void printBuffer(uint8_t col, uint8_t row, const char *str);
    void printBuffer(uint8_t col, uint8_t row, uint8_t col_size, const char *str);
    size_t cPrint(const char *str, const char *color);
    size_t write(const uint8_t *buffer, const uint8_t *color, size_t size);
    size_t write(uint8_t c);
    void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);

    void setRedrawRegion(uint8_t col, uint8_t row, uint8_t width, uint8_t height);
    uint16_t getDataPosition(uint8_t col, uint8_t row);
    uint16_t getNextDataPosition(uint16_t pos);
    uint8_t getDataPositionSize(uint16_t pos);
    
    const char * getColorPointerAtPosition(uint16_t);
    uint16_t getPalletColor(uint8_t, uint8_t);
    void fillArea(uint8_t col, uint8_t row, uint8_t width, uint8_t height, uint16_t c);

    uint8_t scene;
    char screen[801];
    char color[801];
  private:
    struct sprite
    {
      boolean active;
      boolean moved;
      char chr;
      uint8_t pos_x;
      uint8_t pos_y;
      uint8_t pos_x_last;
      uint8_t pos_y_last;
      uint16_t lifetime;
      uint16_t color;
    } sprites[16];
    
    char buffer_char[801];
    uint16_t buffer_color[801];
    
    uint32_t frame;
    
    void setPosition(uint8_t col, uint8_t row);
    void redrawWindow(uint8_t col, uint8_t row, uint8_t width, uint8_t height);
    void drawSelection();
    void splash();
    unsigned long time;
    boolean _enable;
    boolean enableBuffer;
    boolean writeBuffer;
};


extern DisplayClass Display;


#endif
