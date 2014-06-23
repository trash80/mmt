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

#include "route.h"
#include "player.h"
#include "joypad.h"
#include "song.h"
#include "chain.h"

RouteClass::RouteClass()
{
  
}

void RouteClass::begin()
{
  setView(SONG_VIEW);
}

void RouteClass::update()
{
  switch(view) {
    case CHAIN_VIEW:
      Chain.update();
      break;
    case SONG_VIEW:
      Song.update();
      break;
  }
}

void RouteClass::action(uint8_t input)
{
  switch(input) {
    case J_SELECT|J_UP:
    case J_SELECT|J_DOWN:
    case J_SELECT|J_LEFT:
    case J_SELECT|J_RIGHT:
      actionView(input);
      break;
    default:
      switch(view) {
        case CHAIN_VIEW:
          Chain.action(input);
          break;
        case SONG_VIEW:
          Song.action(input);
          break;
      }
  }
}

void RouteClass::actionView(uint8_t input)
{
  
}

void RouteClass::setView(uint8_t v)
{
  switch(view) {
    case CHAIN_VIEW:
      Chain.init();
      break;
    case SONG_VIEW:
      Song.init();
      break;
    default:
      return;
  }
  view = v;
  drawMap();
}

void RouteClass::drawMap()
{
    //Display.setPosition(30,15);
    //Display.printBuffer(0,0,"test");
}

RouteClass Route;
