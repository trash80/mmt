/*
 * Player class File
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

#ifndef _player_h
#define _player_h

#include "Arduino.h"
#include "globals.h"

void clock_start(uint8_t);
void clock_stop();

class PlayerClass {
  public:
    PlayerClass();
    void begin();
    void init();
    void stop();
    void play(uint8_t);
    void play() {
      play(0);
    };
    void playSong(uint8_t);
    void playTrack(uint8_t, uint8_t);
    void stopTrack(uint8_t);

    void playChain(uint8_t, uint8_t);
    void playChain(uint8_t chain) {
      playChain(chain, 0);
    };
    void playPattern(uint8_t, uint8_t);
    void playPattern(uint8_t pattern) {
      playPattern(pattern, 0);
    };

    bool playingChain(uint8_t);
    bool playingPattern(uint8_t);

    bool isSongMode() { return play_mode == 0; };
    bool isChainMode() { return play_mode == 1; };
    bool isPatternMode() { return play_mode == 2; };


    void tick();
    bool playing() { return is_playing; }

    unsigned long position;

    volatile struct groove
    {
        uint8_t position;
        uint8_t ticks;
    } groove;

    volatile struct table
    {
      uint8_t * groove;
      uint8_t step_pos;
      uint8_t tick;
      uint8_t groove_number;
      boolean active;
      boolean playing;
    } table;

    volatile struct track
    {
      volatile uint8_t * groove;
      volatile uint8_t * song;
      volatile uint8_t * chain;
      volatile uint8_t * pattern;
      volatile uint8_t * instrument;
      volatile uint8_t * table;
      volatile uint8_t * table_groove;

      uint8_t table_number;
      uint8_t table_tick;
      uint8_t table_active;
      uint8_t table_groove_number;
      uint8_t play_position;
      uint8_t step_pos;
      uint8_t song_pos;
      uint8_t chain_pos;
      uint8_t table_pos;
      uint8_t last_step_pos;
      uint8_t last_song_pos;
      uint8_t last_chain_pos;
      uint8_t last_table_pos;
      uint8_t last_note;
      uint8_t ch;
      uint8_t tick;
      uint8_t groove_number;
      int8_t kill_counter;
      int8_t delay_counter;
      boolean active;
      boolean playing;
    } tracks[NUM_TRACKS];
    uint8_t tempo;

  private:
    bool is_playing;
    uint8_t play_mode;

};

extern PlayerClass Player;
#endif
