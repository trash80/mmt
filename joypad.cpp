/*
 * Joypad user input class
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

 #include "joypad.h"

JoypadClass::JoypadClass() {
}

void JoypadClass::begin(uint8_t pinA,uint8_t pinB,uint8_t pinStart,uint8_t pinSelect,uint8_t pinRight,uint8_t pinDown, uint8_t pinUp, uint8_t pinLeft)
{
  uint8_t *p =& pins[0]; 
  (*p++) = pinA;
  (*p++) = pinB;
  (*p++) = pinStart;
  (*p++) = pinSelect;
  (*p++) = pinRight;
  (*p++) = pinDown;
  (*p++) = pinUp;
  (*p)   = pinLeft;

  for(uint8_t x=0;x!=8;x++) {
    pinMode(pins[x],INPUT_PULLUP);
  }

  read_waiting= false;

  delay(10); //Delay to ensure internal pull-ups are activated
  setRepeat(300,30);
}

void JoypadClass::setRepeat(unsigned long delayTime, unsigned long repeatTime)
{
  if(delayTime) {
    keyDelay = delayTime;
    keyRepeat= repeatTime;
    keyCheck = repeatTime * 2;
    repeatEnable = true;
  }
}

boolean JoypadClass::available()
{
  if(read_waiting) return true;

  uint8_t joy = 0;
  unsigned long time =  millis();

  for(uint8_t x=0;x!=8;x++) {
    joy |= (!digitalRead(pins[x]))<<(x);
  }
  
  if(joy >= last_state) {
    if(repeatEnable && last_state == joy) {
      if(timer < time) {
        timer = time + keyRepeat;
        touchTimer = time - 1;
        last_state = 0;
        is_repeat  = true;
      }
    } else {
      is_repeat = false;
      touchTimer = time - 1;
      timer = time + keyDelay;
    }
    
    if(last_state != joy && (touchTimer < time)) {
      touchTimer = time + keyCheck;
      last_state = state = joy;
      read_waiting= true;
      return true;
    }
  }
  
  last_state = state = joy;
  return false;
}

uint8_t JoypadClass::read()
{
  uint8_t result = state;
  setPad();
  state = 0;
  read_waiting = false;
  return result;
}

boolean JoypadClass::hasPad(uint8_t value)
{
  return (value & 0xF0) != 0;
}
boolean JoypadClass::hasUp(uint8_t value)
{
  return (value & J_UP) != 0;
}
boolean JoypadClass::hasDown(uint8_t value)
{
  return (value & J_DOWN) != 0;
}
boolean JoypadClass::hasLeft(uint8_t value)
{
  return (value & J_LEFT) != 0;
}
boolean JoypadClass::hasRight(uint8_t value)
{
  return (value & J_RIGHT) != 0;
}
boolean JoypadClass::hasA(uint8_t value)
{
  return (value & J_A) != 0;
}
boolean JoypadClass::hasB(uint8_t value)
{
  return (value & J_B) != 0;
}
boolean JoypadClass::hasStart(uint8_t value)
{
  return (value & J_START) != 0;
}
boolean JoypadClass::hasSelect(uint8_t value)
{
  return (value & J_SELECT) != 0;
}

boolean JoypadClass::isRepeat()
{
  return is_repeat;
}
boolean JoypadClass::isPad(uint8_t value)
{
  return value && (value & 0xF0) == value;
}
boolean JoypadClass::isUp(uint8_t value)
{
  return value == J_UP;
}
boolean JoypadClass::isDown(uint8_t value)
{
  return value == J_DOWN;
}
boolean JoypadClass::isLeft(uint8_t value)
{
  return value == J_LEFT;
}
boolean JoypadClass::isRight(uint8_t value)
{
  return value == J_RIGHT;
}
boolean JoypadClass::isA(uint8_t value)
{
  return value == J_A;
}
boolean JoypadClass::isB(uint8_t value)
{
  return value == J_B;
}
boolean JoypadClass::isStart(uint8_t value)
{
  return value == J_START;
}
boolean JoypadClass::isSelect(uint8_t value)
{
  return value == J_SELECT;
}

void JoypadClass::setPad()
{
  if(hasLeft(state)) {
    pad[0] = -1;
  } else if (hasRight(state)) {
    pad[0] = 1;
  } else {
    pad[0] = 0;
  }
  
  if(hasUp(state)) {
    pad[1] = -1;
  } else if (hasDown(state)) {
    pad[1] = 1;
  } else {
    pad[1] = 0;
  }
}

JoypadClass Joypad;
