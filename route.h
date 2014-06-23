#ifndef route_h
#define route_h

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
  "PPPCC"
  "SCPIT"
  "GGGGG"
;

class RouteClass {
  public:
    RouteClass();
    void begin();
    void action(uint8_t);
    void update();
  	void setView(uint8_t);
  private:
  	void actionView(uint8_t);
    void drawMap();
    uint8_t view;
    uint8_t map[18];
};

extern RouteClass Route;
#endif
