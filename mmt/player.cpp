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

IntervalTimer clock;

void clock_handler()
{
    Player.tick();
}

void clock_start(uint8_t tempo)
{
    clock.priority(0);
    clock.begin(clock_handler,  (60000000 / (tempo*PPQ)));
}

void clock_stop()
{
    clock.end();
}

PlayerClass::PlayerClass()
{
  //data = &memory;
}

void PlayerClass::begin()
{
    for(uint8_t x=0;x!=NUM_TRACKS;x++) {
        for(uint8_t s=0;s<SONG_SIZE;s++) memory.song[s][x] = 255;
    }

    for(uint8_t x=0;x!=CHAIN_SIZE;x++) {
        for(uint8_t r=0;r<ROW_LENGTH;r++) memory.chain[x][r][0] = 255;
    }
    for(uint8_t x=0;x!=PATTERN_SIZE;x++) {
        for(uint8_t r=0;r<ROW_LENGTH;r++) {
            for(uint8_t c=0;c<PATTERN_COLUMNS;c++) {
                memory.pattern[x][r][c] = -1;
            }
        }
    }
    for(uint8_t x=0;x!=GROOVE_SIZE;x++) {
        for(uint8_t r=0;r<ROW_LENGTH;r++) {
            memory.groove[x][r] = -1;
        }
        memory.groove[x][0] = 6;
        memory.groove[x][1] = 6;
    }

    for(uint8_t x=0;x!=NUM_TRACKS;x++) {
        playTrack(x, 0);
        stopTrack(x);
    }
    tempo = 120;
    play_mode = 0;
}

void PlayerClass::init()
{

}

void PlayerClass::stop()
{
    if(!playing()) return ;
    clock_stop();
    for(unsigned char x=0;x!=NUM_TRACKS;x++) {
        stopTrack(x);
    }

    is_playing = false;
}

void PlayerClass::play(uint8_t mode)
{
    if(playing()) return ;
    position = 0;
    clock_start(tempo);

    play_mode = mode;
    is_playing = true;
}

void PlayerClass::playSong(uint8_t pos)
{
    if(pos >= SONG_SIZE) return ;
    
    stop();
    for(uint8_t x=0;x!=NUM_TRACKS;x++) {
        playTrack(x, pos);
    }
    play_mode = 0;

    play();
}

void PlayerClass::playTrack(uint8_t t, uint8_t pos)
{
    if(t >= NUM_TRACKS) return ;
    stop();

    volatile struct track * current_track =& tracks[t];

    if(memory.song[pos][t] >= CHAIN_SIZE) {
        while(memory.song[pos][t] >= CHAIN_SIZE) {
            pos--;
            if(pos == 0xFF || pos >= SONG_SIZE) {
                return ;
            }
        }
    }

    (*current_track).song    =& memory.song[pos][t];

    (*current_track).tick = 0;
    (*current_track).groove_number = 0;
    (*current_track).groove  =& memory.groove[(*current_track).groove_number][0];
    (*current_track).chain   =& memory.chain[*((*current_track).song)][0][0];
    (*current_track).pattern =& memory.pattern[*((*current_track).chain)][0][0];
    (*current_track).song_pos = pos;
    (*current_track).chain_pos = 0;
    (*current_track).step_pos = 0;
    (*current_track).table_pos = 0;
    (*current_track).playing = true;

    (*current_track).play_position = pos;

}

void PlayerClass::stopTrack(uint8_t t)
{
    if(t >= NUM_TRACKS) return ;

    volatile struct track * current_track =& tracks[t];
    if ((*current_track).last_note) {
        Serial.write(0x80+t);
        Serial.write((*current_track).last_note);
        Serial.write(0x7F);
        (*current_track).last_note = 0;

    }
    (*current_track).song_pos = 0;
    (*current_track).chain_pos = 0;
    (*current_track).step_pos = 0;
    (*current_track).table_pos = 0;
    (*current_track).active = false;
    (*current_track).playing = false;
    (*current_track).table_active = false;
}

void PlayerClass::playChain(uint8_t c, uint8_t start_position)
{
    stop();
    
    volatile struct track * current_track =& tracks[0];

    (*current_track).tick = 0;
    (*current_track).groove_number = 0;
    (*current_track).groove  =& memory.groove[(*current_track).groove_number][0];
    (*current_track).chain   =& memory.chain[c][start_position][0];
    (*current_track).pattern =& memory.pattern[*((*current_track).chain)][0][0];
    (*current_track).chain_pos = start_position;
    (*current_track).step_pos = 0;
    (*current_track).table_pos = 0;
    (*current_track).playing = true;

    (*current_track).play_position = c;

    play(1);

}

void PlayerClass::playPattern(uint8_t p, uint8_t start_position)
{
    stop();
    
    volatile struct track * current_track =& tracks[0];
    
    (*current_track).tick = 0;
    (*current_track).groove_number = 0;
    (*current_track).groove  =& memory.groove[(*current_track).groove_number][0];
    (*current_track).pattern =& memory.pattern[p][start_position][0];
    (*current_track).step_pos = start_position;
    (*current_track).table_pos = 0;
    (*current_track).playing = true;

    (*current_track).play_position = p;

    play(2);
}

bool PlayerClass::playingChain(uint8_t c)
{
    volatile struct track * current_track =& tracks[0];

    if(play_mode == 2) {
        return false;
    }

    if(play_mode == 1) {
        return (*current_track).play_position == c;
    }
    
    for(unsigned char x=0;x!=NUM_TRACKS;x++) {
        if((*current_track).playing && *((*current_track).song) == c) {
            return true;
        }
        current_track++;
    }

    return false;
}

bool PlayerClass::playingPattern(uint8_t p)
{
    volatile struct track * current_track =& tracks[0];

    if(play_mode == 2) {
        return (*current_track).play_position == p;
    }

    for(unsigned char x=0;x!=NUM_TRACKS;x++) {
        if((*current_track).playing && *((*current_track).chain) == p) {
            return true;
        }
        current_track++;
    }

    return false;
}

void PlayerClass::tick()
{
    unsigned char hop = 0xFF;
    volatile struct track * current_track =& tracks[0];
    
    for(unsigned char x=0;x!=NUM_TRACKS;x++) {
        hop = 0xFF;
        if((*current_track).playing) {
            if((*current_track).tick == 0) {
                (*current_track).active = true;

                if(*((*current_track).pattern+PATTERN_EFFECT_COL1) == EFFECT_HOP) {
                    hop = *((*current_track).pattern+PATTERN_EFFECT_VAL1);
                }

                if(*((*current_track).pattern+PATTERN_EFFECT_COL2) == EFFECT_HOP) {
                    hop = *((*current_track).pattern+PATTERN_EFFECT_VAL2);
                }

                if((*current_track).step_pos == ROW_LENGTH || hop != 0xFF) {
                    if(play_mode == 2) {
                        if(hop == 0xFF) {
                            (*current_track).step_pos = 0;
                            (*current_track).pattern = & memory.pattern[(*current_track).play_position][0][0];
                        } else {
                            (*current_track).step_pos = hop;
                            (*current_track).pattern-= (PATTERN_COLUMNS * ROW_LENGTH);
                        }
                    } else {
                        (*current_track).chain+=CHAIN_COLUMNS;
                        (*current_track).last_chain_pos = (*current_track).chain_pos;
                        (*current_track).chain_pos++;

                        if((*(*current_track).chain) == 0xFF) {
                            
                            if(play_mode == 1) {
                                (*current_track).chain =& memory.chain[(*current_track).play_position][0][0];
                                (*current_track).chain_pos = 0;
                            } else {
                                (*current_track).last_song_pos = (*current_track).song_pos;
                                (*current_track).song_pos++;
                                (*current_track).song =& memory.song[(*current_track).song_pos][x];

                                if((*(*current_track).song) == 0xFF) {
                                    (*current_track).song_pos = (*current_track).play_position;
                                    (*current_track).song =& memory.song[(*current_track).song_pos][x];

                                    if(((*current_track).song_pos) != 0xFF) {
                                        while(
                                            (*current_track).song_pos < SONG_SIZE && (*(*current_track).song) != 0xFF) {
                                            (*current_track).song_pos--;
                                            (*current_track).song =& memory.song[(*current_track).song_pos][x];
                                        }
                                        (*current_track).song_pos++;
                                        (*current_track).song =& memory.song[(*current_track).song_pos][x];
                                    }

                                }
                                (*current_track).chain =& memory.chain[*((*current_track).song)][0][0];
                                (*current_track).chain_pos = 0;
                            }
                        }

                        (*current_track).pattern =& memory.pattern[*((*current_track).chain)][0][0];
                    
                    }
                    if(hop == 0xFF) {
                        (*current_track).step_pos = 0;
                    } else {
                        hop &= 0x0F;
                        (*current_track).step_pos = hop;
                        (*current_track).pattern += (hop * PATTERN_COLUMNS);
                    }
                }

                if(play_mode != 2 && (*(*current_track).chain) == 0xFF) {
                    (*current_track).playing = false;
                    current_track++;
                    continue ;
                }

                //Instrument
                if(*((*current_track).pattern+PATTERN_INS)) {
                    (*current_track).instrument =& memory.instrument[*((*current_track).pattern+PATTERN_INS)][0];

                    if(*((*current_track).instrument+INS_TABLE)) {
                        (*current_track).table =& memory.table[*((*current_track).instrument+INS_TABLE)][0][0];
                        (*current_track).table_groove_number = 127;
                        (*current_track).table_groove =& memory.groove[(*current_track).table_groove_number][0];
                        (*current_track).table_tick = 0;
                        (*current_track).table_active = true;
                    }
                }

                //Table
                if(*((*current_track).pattern+PATTERN_EFFECT_COL1) == EFFECT_TABLE) {
                    (*current_track).table =& memory.table[*((*current_track).pattern+PATTERN_EFFECT_VAL1)][0][0];
                    (*current_track).table_groove_number = 127;
                    (*current_track).table_groove =& memory.groove[(*current_track).table_groove_number][0];
                    (*current_track).table_tick = 0;
                    (*current_track).table_active = true;
                }
                if(*((*current_track).pattern+PATTERN_EFFECT_COL2) == EFFECT_TABLE) {
                    (*current_track).table =& memory.table[*((*current_track).pattern+PATTERN_EFFECT_VAL2)][0][0];
                    (*current_track).table_groove_number = 127;
                    (*current_track).table_groove =& memory.groove[(*current_track).table_groove_number][0];
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
                    if((*(*current_track).table_groove) == 0xFF) {
                        (*current_track).table_groove =& memory.groove[(*current_track).table_groove_number][0];
                    }
                }
                
                (*current_track).table_tick++;
                
                if((*current_track).table_tick == *(*current_track).table_groove) {
                    (*current_track).table_tick = 0;
                    (*current_track).table_groove++;
                    if((*(*current_track).table_groove) == 0xFF) {
                        (*current_track).table_groove =& memory.groove[(*current_track).table_groove_number][0];
                    }
                }
            }


            if((*current_track).active) {
                if(*((*current_track).pattern+PATTERN_NOTE) < 128) {
                    //Note is set
                    if ((*current_track).last_note < 128) {
                        Serial.write(0x80+(*current_track).ch);
                        Serial.write((*current_track).last_note & 0x7f);
                        Serial.write(0x7F);
                        (*current_track).last_note = 0;
                    }
                    
                    if(*((*current_track).pattern+PATTERN_INS) < 128) (*current_track).ch = *((*current_track).pattern+PATTERN_INS);
                    
                    Serial.write(0x90+(*current_track).ch);
                    Serial.write((*((*current_track).pattern) + (signed char) *((*current_track).chain+CHAIN_TRANSPOSE)) & 0x7f);
                    Serial.write(*((*current_track).pattern+PATTERN_VEL) & 0x7F);

                    (*current_track).last_note = (*((*current_track).pattern) + (signed char) *((*current_track).chain+CHAIN_TRANSPOSE)) & 0x7f;
                }
                (*current_track).active = false;
            }

            (*current_track).tick++;

            if((*current_track).tick == *(*current_track).groove) {
                (*current_track).tick = 0;
                (*current_track).pattern+=PATTERN_COLUMNS;
                (*current_track).last_step_pos = (*current_track).step_pos;
                (*current_track).step_pos++;
                (*current_track).groove++;
                if((*(*current_track).groove) == 0xFF) {
                    (*current_track).groove =& memory.groove[(*current_track).groove_number][0];
                }
            }
        }
        current_track++;
    }
    position++;
}




PlayerClass Player;
