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

void inputCallback()
{
  Joypad.callback();
}

JoypadClass::JoypadClass() {
}

void JoypadClass::begin()
{

  pinMode(6, INPUT);

  pinA      = 8;
  pinB      = 9;
  pinStart  = 10;
  pinSelect = 11;
  pinRight  = 12;
  pinDown   = 13;
  pinUp     = 14;
  pinLeft   = 15;
  
  mcp.begin();
  mcp.pinMode(0, INPUT);
  mcp.pullUp(0, HIGH);
  mcp.setupInterrupts(true,false,LOW);

  mcp.pinMode(pinUp, INPUT);
  mcp.pullUp(pinUp, HIGH);
  mcp.pinMode(pinDown, INPUT);
  mcp.pullUp(pinDown, HIGH);
  mcp.pinMode(pinLeft, INPUT);
  mcp.pullUp(pinLeft, HIGH);
  mcp.pinMode(pinRight, INPUT);
  mcp.pullUp(pinRight, HIGH);
  mcp.pinMode(pinSelect, INPUT);
  mcp.pullUp(pinSelect, HIGH);
  mcp.pinMode(pinStart, INPUT);
  mcp.pullUp(pinStart, HIGH);
  mcp.pinMode(pinA, INPUT);
  mcp.pullUp(pinA, HIGH);
  mcp.pinMode(pinB, INPUT);
  mcp.pullUp(pinB, HIGH);

  bufferPosition = readPosition = 0;
  callback();
  bufferPosition--;

  attachInterrupt(6,inputCallback,FALLING);

  delay(10); //Delay to ensure internal pull-ups are activated
  setRepeat(300,30);
}

void JoypadClass::callback()
{
  uint8_t c = (uint8_t) ((mcp.readGPIOAB()>>8)) & 0xFF;
  if(wait_clear && c == 0xFF) {
    wait_clear = false;
  } else if (!wait_clear) {
    bufferPosition += 1;
    if (bufferPosition > 7) bufferPosition = 0;
    buffer[bufferPosition] = 0xFF - c;
  }

  mcp.setupInterruptPin(pinA,CHANGE);
  mcp.setupInterruptPin(pinB,CHANGE);
  mcp.setupInterruptPin(pinStart,CHANGE);
  mcp.setupInterruptPin(pinSelect,CHANGE);
  mcp.setupInterruptPin(pinUp,CHANGE);
  mcp.setupInterruptPin(pinDown,CHANGE);
  mcp.setupInterruptPin(pinLeft,CHANGE);
  mcp.setupInterruptPin(pinRight,CHANGE);
}

void JoypadClass::setRepeat(unsigned long delayTime, unsigned long repeatTime)
{
  if(delayTime) {
    keyDelay = delayTime;
    keyRepeat= repeatTime;
    repeatEnable = true;
  }
}

bool JoypadClass::available()
{
  if (bufferPosition != readPosition) {
    touchTimer = millis() + keyDelay;
    is_repeat = false;
    return true;
  }

  if(repeatEnable && hasPad(buffer[readPosition]) && millis() > touchTimer) {
    is_repeat = true;
    touchTimer = millis() + keyRepeat;
    return true;
  }
  return false;
}

uint8_t JoypadClass::read()
{
  if(readPosition != bufferPosition) {
    readPosition+=1;
    if(readPosition > 7) readPosition = 0;
  }
  setPad();
  return buffer[readPosition];
}

void JoypadClass::waitClear()
{
  wait_clear = true;
}

boolean JoypadClass::hasPad(uint8_t value)
{
  return (value > 8);
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
  if(hasLeft(buffer[readPosition])) {
    pad[0] = -1;
  } else if (hasRight(buffer[readPosition])) {
    pad[0] = 1;
  } else {
    pad[0] = 0;
  }

  if(hasUp(buffer[readPosition])) {
    pad[1] = -1;
  } else if (hasDown(buffer[readPosition])) {
    pad[1] = 1;
  } else {
    pad[1] = 0;
  }
}

JoypadClass Joypad;
