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
    case PATTERN_VIEW:
      Pattern.update();
      break;
  }
}

void RouteClass::draw()
{
  switch(view) {
    case CHAIN_VIEW:
      Chain.draw();
      break;
    case SONG_VIEW:
      Song.draw();
      break;
    case PATTERN_VIEW:
      Pattern.draw();
      break;
  }
  char t[4];
  sprintf(t,"%u",Player.tempo);
  Display.printBuffer(34,1,t);
  //Display.printNote(&Display.screen[],*data);
}

void RouteClass::action(uint8_t input)
{
  switch(view) {
    case CHAIN_VIEW:
      Chain.action(input);
      break;
    case SONG_VIEW:
      Song.action(input);
      break;
    case PATTERN_VIEW:
      Pattern.action(input);
      break;
  }
}

void RouteClass::setView(uint8_t v)
{
  switch(v) {
    case CHAIN_VIEW:
      Chain.init();
      break;
    case SONG_VIEW:
      Song.init();
      break;
    case PATTERN_VIEW:
      Pattern.init();
      break;
    default:
      return;
  }
  view = v;
  //drawMap();
}

void RouteClass::drawMap()
{
    Display.printBuffer(32,16,6,"      SCPIT       ");
}

RouteClass Route;
