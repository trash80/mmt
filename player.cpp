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
 
 #include "player.h"

uint8_t m_project[PROJECT_SIZE];
uint8_t m_song[SONG_SIZE][NUM_TRACKS];
uint8_t m_pattern[MEM_SIZE][ROW_LENGTH][PATTERN_COLUMNS];
uint8_t m_chain[MEM_SIZE][ROW_LENGTH][CHAIN_COLUMNS];
uint8_t m_instrument[MEM_SIZE][INS_SIZE];
uint8_t m_table[MEM_SIZE][ROW_LENGTH][TABLE_COLUMNS];
uint8_t m_profile[NUM_PROFILES][ROW_LENGTH];
uint8_t m_groove[MEM_SIZE][ROW_LENGTH];
uint8_t m_copy_buffer[256];

IntervalTimer clock;

void clock_handler()
{
    Player.tick();
}

void clock_start(uint8_t bpm)
{
    clock.begin(clock_handler,  (60000000 / (bpm*PPQ)));
}

void clock_stop(uint8_t bpm)
{
    clock.end();
}

PlayerClass::PlayerClass()
{
  //data = &memory;
}

void PlayerClass::begin()
{
  struct track * current_track =& tracks[0];
  for(uint8_t x=0;x!=NUM_TRACKS;x++) {
      (*current_track).tick = 0;
      (*current_track).groove  =& m_groove[(*current_track).groove_number][0];
      (*current_track).song    =& m_song[x][0];
      (*current_track).chain   =& m_chain[*((*current_track).song)][0][0];
      (*current_track).pattern =& m_pattern[*((*current_track).chain)][0][0];
      (*current_track).song_start_pos = 0;
      (*current_track).song_pos = 0;
      (*current_track).chain_pos = 0;
      (*current_track).step_pos = 0;
      (*current_track).table_pos = 0;
      (*current_track).active = true;
      current_track++;
  }
  clock_start(120);
}

void PlayerClass::playSong(uint8_t pos)
{
  
}

void PlayerClass::stop()
{
  
}

void PlayerClass::playTrack(uint8_t track, uint8_t pos)
{
  
}

void PlayerClass::stopTrack(uint8_t track)
{
  
}

void PlayerClass::playChain(uint8_t track, uint8_t chain)
{

}

void PlayerClass::playPattern(uint8_t track, uint8_t pattern)
{
  
}

void PlayerClass::tick()
{
    unsigned char hop = 0xFF;
    struct track * current_track =& tracks[0];
    for(unsigned char x=0;x!=NUM_TRACKS;x++) {
        hop = 0xFF;
        if((*current_track).playing) {
          if((*current_track).tick == 0) {
              (*current_track).active = true;
              (*current_track).pattern+=PATTERN_COLUMNS;

              if(*((*current_track).pattern+PATTERN_EFFECT_COL1) == EFFECT_HOP) {
                  hop = *((*current_track).pattern+PATTERN_EFFECT_VAL1);
              }
              if(*((*current_track).pattern+PATTERN_EFFECT_COL2) == EFFECT_HOP) {
                  hop = *((*current_track).pattern+PATTERN_EFFECT_VAL2);
              }
              
              (*current_track).last_step_pos = (*current_track).step_pos;
              (*current_track).step_pos++;
              
              if((*current_track).step_pos == ROW_LENGTH || hop != 0xFF) {
                  (*current_track).chain+=CHAIN_COLUMNS;
                  (*current_track).last_chain_pos = (*current_track).chain_pos;
                  (*current_track).chain_pos++;

                  if(!(*(*current_track).chain)) {
                      (*current_track).last_song_pos = (*current_track).song_pos;
                      (*current_track).song_pos++;
                      (*current_track).song++;
                      
                      if(!(*(*current_track).song)) {
                          (*current_track).song_pos = (*current_track).song_start_pos;
                          (*current_track).song =& m_song[x][(*current_track).song_pos];
                          
                          if((*current_track).song_pos) {
                              while((*(*current_track).song)) {
                                (*current_track).song--;
                                (*current_track).song_pos--;
                              }
                              (*current_track).song++;
                              (*current_track).song_pos++;
                          }
                          
                      }
                      
                      (*current_track).chain =& m_chain[*((*current_track).song)][0][0];
                      (*current_track).chain_pos = 0;
                  }
                  (*current_track).pattern =& m_pattern[*((*current_track).chain)][0][0];
                  if(hop == 0xFF) {
                      (*current_track).step_pos = 0;
                  } else {
                      (*current_track).step_pos = hop;
                  }
              }
              if(!(*(*current_track).chain)) {
                  (*current_track).playing = false;
              } else {
                  (*current_track).playing = true;
              }

              //Instrument
              if(*((*current_track).pattern+PATTERN_INS)) {
                  (*current_track).instrument =& m_instrument[*((*current_track).pattern+PATTERN_INS)][0];
                  
                  if(*((*current_track).instrument+INS_TABLE)) {
                      (*current_track).table =& m_table[*((*current_track).instrument+INS_TABLE)][0][0];
                      (*current_track).table_groove_number = 127;
                      (*current_track).table_groove =& m_groove[(*current_track).table_groove_number][0];
                      (*current_track).table_tick = 0;
                      (*current_track).table_active = true;
                  }
              }

              //Table
              if(*((*current_track).pattern+PATTERN_EFFECT_COL1) == EFFECT_TABLE) {
                  (*current_track).table =& m_table[*((*current_track).pattern+PATTERN_EFFECT_VAL1)][0][0];
                  (*current_track).table_groove_number = 127;
                  (*current_track).table_groove =& m_groove[(*current_track).table_groove_number][0];
                  (*current_track).table_tick = 0;
                  (*current_track).table_active = true;
              }
              if(*((*current_track).pattern+PATTERN_EFFECT_COL2) == EFFECT_TABLE) {
                  (*current_track).table =& m_table[*((*current_track).pattern+PATTERN_EFFECT_VAL2)][0][0];
                  (*current_track).table_groove_number = 127;
                  (*current_track).table_groove =& m_groove[(*current_track).table_groove_number][0];
                  (*current_track).table_tick = 0;
                  (*current_track).table_active = true;
              }
          }


          if((*current_track).table_active) {
              if((*current_track).table_tick == *(*current_track).table_groove) {
                  //Do table
                  if(*((*current_track).table+TABLE_NOTE)) {
                  }
                  if(*((*current_track).table+TABLE_VEL)) {
                  }





                  //Rewind table
                  (*current_track).table_pos++;
                  (*current_track).table++;
                  if((*current_track).table_pos == ROW_LENGTH) {
                      (*current_track).table -= (*current_track).table_pos;
                      (*current_track).table_pos = 0;
                  } else {
                      //Check for hop
                  }
                  (*current_track).table_groove++;
                  if(!(*(*current_track).table_groove)) {
                      (*current_track).table_groove =& m_groove[(*current_track).table_groove_number][0];
                  }
              }
              (*current_track).table_tick++;
              if((*current_track).table_tick == *(*current_track).table_groove) {
                  (*current_track).table_tick = 0;
                  (*current_track).table_groove++;
                  if(!(*(*current_track).table_groove)) {
                      (*current_track).table_groove =& m_groove[(*current_track).table_groove_number][0];
                  }
              }

          } else if((*current_track).active) {
              if(*((*current_track).pattern+PATTERN_NOTE)) {
                  //Note is set


                  


                  if ((*current_track).last_note) {
                      Serial.write(0x80+x);
                      Serial.write((*current_track).last_note);
                      Serial.write(0x7F);
                      (*current_track).last_note = 0;
                  }

                  Serial.write(0x90+x);
                  Serial.write(*((*current_track).pattern) + (signed char) *((*current_track).chain+CHAIN_TRANSPOSE));
                  Serial.write(*((*current_track).pattern+2));
                  (*current_track).last_note = *((*current_track).pattern) + (signed char) *((*current_track).chain+CHAIN_TRANSPOSE);
              }

              (*current_track).active = false;
          }

          (*current_track).tick++;
          
          if((*current_track).tick == *(*current_track).groove) {
              (*current_track).tick = 0;
              (*current_track).groove++;
              if(!(*(*current_track).groove)) {
                  (*current_track).groove =& m_groove[(*current_track).groove_number][0];
              }
          }

        }
        current_track++;
    }
}


void PlayerClass::init()
{
  
}



PlayerClass Player;
