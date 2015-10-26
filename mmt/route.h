/*
 * Route class File - Routes user input & updates to controllers
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

#ifndef route_h
#define route_h

#include "display.h"
#include "player.h"
#include "joypad.h"
#include "views/song.h"
#include "views/chain.h"
#include "views/pattern.h"
#include "Arduino.h"

#define SONG_VIEW 0
#define GROOVE_VIEW 1
#define CHAIN_VIEW 2
#define PATTERN_VIEW 3
#define INS_VIEW 4
#define TABLE_VIEW 5
#define PROFILE_VIEW 6
#define PROJECT_VIEW 7
#define LOAD_VIEW 8
#define PERF_VIEW 9

static const unsigned char routeMap[15] PROGMEM = {
  PROJECT_VIEW, PROJECT_VIEW, PROJECT_VIEW, PROFILE_VIEW, PROFILE_VIEW,
  SONG_VIEW,    CHAIN_VIEW,   PATTERN_VIEW, INS_VIEW,     TABLE_VIEW, 
  GROOVE_VIEW,  GROOVE_VIEW,  GROOVE_VIEW,  GROOVE_VIEW,  GROOVE_VIEW,
};

static const char routeMapChr[16] PROGMEM = 
  "P~~C~"
  "SCPIT"
  "G~~~~"
;

class RouteClass {
  public:
    RouteClass();
    void begin();
    void action(uint8_t);
    void update();
    void draw();
  	void setView(uint8_t);
  private:
  	void actionView(uint8_t);
    void drawMap();
    uint8_t view;
    uint8_t map[18];
};

extern RouteClass Route;
#endif
