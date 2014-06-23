
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
