/*
 * MMT - A music tracker for Cortex-M4 / Teensy 3.1 & TFT ILI9341
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
