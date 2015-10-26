
#ifndef _MEM
#define _MEM

#include "Arduino.h"

#define PPQ 24
#define NUM_TRACKS 8
#define ROW_LENGTH 16
#define CHAIN_COLUMNS 2
#define PROJECT_SIZE 32

#define SONG_SIZE 255
#define GROOVE_SIZE 16
#define CHAIN_SIZE 128

#define PATTERN_SIZE 255
#define PATTERN_COLUMNS 7
#define PATTERN_NOTE 0
#define PATTERN_VEL  1
#define PATTERN_INS  2
#define PATTERN_EFFECT_COL1 3
#define PATTERN_EFFECT_VAL1 4
#define PATTERN_EFFECT_COL2 5
#define PATTERN_EFFECT_VAL2 6

#define TABLE_SIZE 32
#define TABLE_COLUMNS 8
#define TABLE_NOTE 0
#define TABLE_VEL  1
#define TABLE_EFFECT_COL1 2
#define TABLE_EFFECT_VAL1 3
#define TABLE_EFFECT_COL2 4
#define TABLE_EFFECT_VAL2 5
#define TABLE_EFFECT_COL3 6
#define TABLE_EFFECT_VAL3 7

#define INS_SIZE 64
#define INS_COLUMNS 16
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

#define PROFILE_SIZE 32
#define PROFILE_COLUMNS 68
#define PROFILE_NAME 0
#define PROFILE_NAME_SIZE 8
#define PROFILE_VALUE 8
#define PROFILE_NUM_VALUES 15
#define PROFILE_VALUE_LABEL 9
#define PROFILE_VALUE_LABEL_SIZE 4

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


struct Memory
{
  uint8_t project[PROJECT_SIZE];
  uint8_t groove[GROOVE_SIZE][ROW_LENGTH];
  uint8_t song[SONG_SIZE][NUM_TRACKS];
  uint8_t pattern[PATTERN_SIZE][ROW_LENGTH][PATTERN_COLUMNS];
  uint8_t chain[CHAIN_SIZE][ROW_LENGTH][CHAIN_COLUMNS];
  uint8_t table[TABLE_SIZE][ROW_LENGTH][TABLE_COLUMNS];
  uint8_t instrument[INS_SIZE][INS_COLUMNS];
  uint8_t profile[PROFILE_SIZE][PROFILE_COLUMNS];
  uint8_t copy_buffer[256];
} ;

extern volatile struct Memory memory;
#endif
