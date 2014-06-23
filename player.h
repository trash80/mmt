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

#define PPQ 24
#define NUM_TRACKS 8
#define NUM_PROFILES 32
#define ROW_LENGTH 16
#define MEM_SIZE 128
#define CHAIN_COLUMNS 2
#define PROJECT_SIZE 32

#define SONG_SIZE 256

#define PATTERN_COLUMNS 7
#define PATTERN_NOTE 0
#define PATTERN_VEL  1
#define PATTERN_INS  2
#define PATTERN_EFFECT_COL1 3
#define PATTERN_EFFECT_VAL1 4
#define PATTERN_EFFECT_COL2 5
#define PATTERN_EFFECT_VAL2 6

#define TABLE_COLUMNS 8
#define TABLE_NOTE 0
#define TABLE_VEL  1
#define TABLE_EFFECT_COL1 2
#define TABLE_EFFECT_VAL1 3
#define TABLE_EFFECT_COL2 4
#define TABLE_EFFECT_VAL2 5
#define TABLE_EFFECT_COL3 6
#define TABLE_EFFECT_VAL3 7

#define INS_SIZE 32
#define INS_NAME  0
#define INS_NAME_SIZE 8
#define INS_PORT 8
#define INS_CHANNEL 9
#define INS_TRANSPOSE 10
#define INS_VELOCITY 11
#define INS_PROGRAM 12
#define INS_BANK 13
#define INS_PROFILE 14
#define INS_TABLE 15

#define CHAIN_PATTERN 0
#define CHAIN_TRANSPOSE 1

#define EFFECT_TABLE        5
#define EFFECT_ARPEGGIATO   1
#define EFFECT_CHORD        2
#define EFFECT_DELAY        3
#define EFFECT_ENVELOPE     4
#define EFFECT_VOLUME       EFFECT_ENVELOPE
#define EFFECT_FRAME        5
#define EFFECT_GROOVE       6
#define EFFECT_SET_GROOVE   EFFECT_GROOVE
#define EFFECT_HOP          7
#define EFFECT_KILL_NOTE    8
#define EFFECT_KILL     EFFECT_KILL_NOTE
#define EFFECT_SLIDE        9
#define EFFECT_SET_MASTER_VOLUME    10
#define EFFECT_PAN          11
#define EFFECT_OUTPUT       EFFECT_PAN
#define EFFECT_PITCH        12
#define EFFECT_RETRIG       13
#define EFFECT_SWEEP        14
#define EFFECT_TEMPO        15
#define EFFECT_VIBRATO      16
#define EFFECT_WAVE         17
#define EFFECT_RANDOMIZE    18

extern uint8_t m_project[PROJECT_SIZE];
extern uint8_t m_song[SONG_SIZE][NUM_TRACKS];
extern uint8_t m_pattern[MEM_SIZE][ROW_LENGTH][PATTERN_COLUMNS];
extern uint8_t m_chain[MEM_SIZE][ROW_LENGTH][CHAIN_COLUMNS];
extern uint8_t m_instrument[MEM_SIZE][INS_SIZE];
extern uint8_t m_table[MEM_SIZE][ROW_LENGTH][TABLE_COLUMNS];
extern uint8_t m_profile[NUM_PROFILES][ROW_LENGTH];
extern uint8_t m_profile_names[NUM_PROFILES][ROW_LENGTH][4];
extern uint8_t m_groove[MEM_SIZE][ROW_LENGTH];
extern uint8_t m_copy_buffer[256];

void clock_start(uint8_t);
void clock_stop();

class PlayerClass {
  public:
    PlayerClass();
    void begin();
    void init();
    void playSong(uint8_t);
    void stop();
    void playTrack(uint8_t, uint8_t);
    void stopTrack(uint8_t);
    void playChain(uint8_t, uint8_t);
    void playPattern(uint8_t, uint8_t);
    void tick();
  private:
    struct groove
    {
        uint8_t position;
        uint8_t ticks;
    } groove;
    
    struct table
    {
      uint8_t * groove;
      uint8_t step_pos;
      uint8_t tick;
      uint8_t groove_number;
      boolean active;
      boolean playing;
    } table;
    
    struct track
    {
      uint8_t * groove;
      uint8_t * song;
      uint8_t * chain;
      uint8_t * pattern;
      uint8_t * instrument;
      uint8_t * table;
      uint8_t * table_groove;

      uint8_t table_number;
      uint8_t table_tick;
      uint8_t table_active;
      uint8_t table_groove_number;
      uint8_t step_pos;
      uint8_t song_pos;
      uint8_t song_start_pos;
      uint8_t chain_pos;
      uint8_t table_pos;
      uint8_t last_step_pos;
      uint8_t last_song_pos;
      uint8_t last_chain_pos;
      uint8_t last_table_pos;
      uint8_t last_note;
      uint8_t tick;
      uint8_t groove_number;
      int8_t kill_counter;
      int8_t delay_counter;
      boolean active;
      boolean playing;
    } tracks[NUM_TRACKS];
};

extern PlayerClass Player;
#endif
