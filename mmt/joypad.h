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

#ifndef joypad_h
#define joypad_h
#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include "Arduino.h"

#define J_A 0x01
#define J_B 0x02
#define J_START 0x04
#define J_SELECT 0x08
#define J_RIGHT 0x10
#define J_DOWN 0x20
#define J_UP 0x40
#define J_LEFT 0x80

class JoypadClass {
  public:
    int8_t pad[2];
    JoypadClass();
    void begin();
    boolean available();
    uint8_t read();
    void setRepeat(unsigned long, unsigned long);
    boolean isA(uint8_t);
    boolean isB(uint8_t);
    boolean isStart(uint8_t);
    boolean isSelect(uint8_t);
    boolean isPad(uint8_t);
    boolean isRight(uint8_t);
    boolean isDown(uint8_t);
    boolean isUp(uint8_t);
    boolean isLeft(uint8_t);
    boolean isRepeat();
    boolean hasA(uint8_t);
    boolean hasB(uint8_t);
    boolean hasStart(uint8_t);
    boolean hasSelect(uint8_t);
    boolean hasPad(uint8_t);
    boolean hasRight(uint8_t);
    boolean hasDown(uint8_t);
    boolean hasUp(uint8_t);
    boolean hasLeft(uint8_t);
    void callback();
    void waitClear();
  private:

    Adafruit_MCP23017 mcp;
    
    uint8_t buffer[8];
    uint8_t bufferPosition;
    uint8_t readPosition;
    int pinStart;
    int pinSelect;
    int pinUp;
    int pinDown;
    int pinLeft;
    int pinRight;
    int pinA;
    int pinB;
    bool wait_clear;

    boolean is_repeat;
    unsigned long keyDelay;
    unsigned long keyRepeat = 50;
    unsigned long keyCheck = 100;
    unsigned long timer;
    unsigned long touchTimer;
    boolean repeatEnable;
    void setPad();
};

extern JoypadClass Joypad;
#endif
