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

#include "globals.h"
#include "player.h"
#include "joypad.h"
#include "display.h"
#include "route.h"

volatile struct Memory memory;

void setup()
{
  Serial.begin(9600);
  Serial1.begin(31250);
  Serial2.begin(31250);
  Serial3.begin(31250);
  Joypad.begin();

  pinMode(3,OUTPUT);
  digitalWrite(3, LOW);

  Display.begin();
  digitalWrite(3, HIGH);
  Player.begin();
  Display.splash();
  Route.begin();
  Song.begin();
  Chain.begin();
  Pattern.begin();
}

void loop()
{
  if(Joypad.available()) {
    Route.action(Joypad.read());
  }
  Route.update();
  Display.draw();
}
