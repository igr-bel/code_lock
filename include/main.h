#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

//------------------------------------------------------------------------------------------------------
//                                        INCLUDDES
//------------------------------------------------------------------------------------------------------
#include "functions.h"

//------------------------------------------------------------------------------------------------------
//                                        DEFINES
//------------------------------------------------------------------------------------------------------
//STATE MACHINE
#define MAIN_WORK_STATE				0
#define PROG_STATE					1
#define RIGHT_CODE_STATE			2
#define WRONG_CODE_STATE			3

//------------------------------------------------------------------------------------------------------
//                                        GLOBAL VARIABLES
//------------------------------------------------------------------------------------------------------
//-------------------------------------- ENUMS / STRUCTS
keypad_T current_key;

//-------------------------------------- STATE MACHINE
volatile uint8_t current_state;

//-------------------------------------- VARIABLES
unsigned long work_timer = 0;

const uint8_t MAX_INPUT_CODE_LENGTH = 20;

uint8_t code_length;
const uint8_t code_length_addr = 0;
uint8_t right_input_code[MAX_INPUT_CODE_LENGTH] = {0xFF};
const uint8_t code_addr_offset = 1;

uint8_t code_counter = 0;
uint8_t input_code[MAX_INPUT_CODE_LENGTH] = {0};

// for in prog mode {'*', '2', '5', '8', '#'}
const uint8_t PROG_MODE_LENGTH = 5;
const uint8_t MASTER_MODE_LENGTH = 5;
uint8_t prog_mode_code[PROG_MODE_LENGTH] = {_S, _2, _5, _8, _N};
// master code {'*', '9', '9', '#', '#'}
uint8_t master_code[MASTER_MODE_LENGTH] = {_S, _9, _9, _N, _N};

unsigned long prog_leds_timer = 0;
bool prog_leds_flag = false;

//------------------------------------------------------------------------------------------------------
//                                        DEBUG
//------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------
//                                        FLAGS
//------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------
#endif //MAIN_H_INCLUDED