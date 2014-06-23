#include "SPI.h"
#include "Adafruit_GFX.h"
#include "TFT_ILI9340.h"

#include "player.h"
#include "joypad.h"
#include "display.h"
#include "route.h"

void setup()
{
  Display.begin();
  Joypad.begin(15,16,17,18,19,20,21,22);
  Serial.begin(9600);
  Route.begin();
  Player.begin();
}

void loop()
{
  if(Joypad.available()) 
    Route.action(Joypad.read());
  
  Route.update();
  Display.draw();
}
