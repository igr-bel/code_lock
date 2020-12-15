#ifndef _KEYPAD_H
#define _KEYPAD_H

#include <Arduino.h>

//------------------------------------------------------------------------------------------------------
//                                        DEFINES
//------------------------------------------------------------------------------------------------------
//KEYPAD
#define ROW0                            A1
#define ROW1                            A0
#define ROW2                            13
#define ROW3                            12
#define COL0                            11
#define COL1                            8
#define COL2                            5
#define NUM_OF_ROWS                     4
#define NUM_OF_COLUMNS                  3

#define _1                              1
#define _2                              2
#define _3                              3
#define _4                              4
#define _5                              5
#define _6                              6
#define _7                              7
#define _8                              8
#define _9                              9
#define _0                              10
#define _S                              11
#define _N                              12
#define _NONE                           13

//------------------------------------------------------------------------------------------------------
//                                        GLOBAL VARIABLES
//------------------------------------------------------------------------------------------------------
//GPIO
uint8_t row[NUM_OF_ROWS];
uint8_t column[NUM_OF_COLUMNS];

enum
{
    PRESSED,
    RELEASED,
    HOLD
};

typedef struct keypad
{
    uint8_t key_code;
    uint8_t state;
} keypad_T;

//------------------------------------------------------------------------------------------------------
//                                        PROTOTYPES
//------------------------------------------------------------------------------------------------------
inline void keypadInit(void);
inline keypad_T getKey(void);
inline uint8_t scanRows(void);

//------------------------------------------------------------------------------------------------------
//                                       BEHAVIOR
//------------------------------------------------------------------------------------------------------
inline void keypadInit(void)
{
    row[0] = ROW0;
    row[1] = ROW1;
    row[2] = ROW2;
    row[3] = ROW3;
    column[0] = COL0;
    column[1] = COL1;
    column[2] = COL2; 
    
    for(uint8_t i = 0; i < NUM_OF_ROWS; i++)
    {
        pinMode(row[i], INPUT_PULLUP);
    }

    for(uint8_t i = 0; i < NUM_OF_COLUMNS; i++)
    {
        pinMode(column[i], OUTPUT); 
        digitalWrite(column[i], HIGH);
    }

}//keypadInit()
//------------------------------------------------------------------------------------------------------
inline keypad_T getKey(void)
{
	static keypad_T _prew_key;
	static keypad_T _current_key;

    _current_key.key_code = _NONE;

    //col 1
    digitalWrite(column[0], LOW);
    digitalWrite(column[1], HIGH);
    digitalWrite(column[2], HIGH);
    switch(scanRows())
    {
        case 0:
            _current_key.key_code = _1;
            break;
        case 1:
            _current_key.key_code = _4;
            break;
        case 2:
            _current_key.key_code = _7;
            break;
        case 3:
            _current_key.key_code = _S;
            break;
    }

    //col 2
    digitalWrite(column[0], HIGH);
    digitalWrite(column[1], LOW);
    digitalWrite(column[2], HIGH);
    switch(scanRows())
    {
        case 0:
            _current_key.key_code = _2;
            break;
        case 1:
            _current_key.key_code = _5;
            break;
        case 2:
            _current_key.key_code = _8;
            break;
        case 3:
            _current_key.key_code = _0;
            break;
    }

    //col 3
    digitalWrite(column[0], HIGH);
    digitalWrite(column[1], HIGH);
    digitalWrite(column[2], LOW);
    switch(scanRows())
    {
        case 0:
            _current_key.key_code = _3;
            break;
        case 1:
            _current_key.key_code = _6;
            break;
        case 2:
            _current_key.key_code = _9;
            break;
        case 3:
            _current_key.key_code = _N;
            break;
    }

    if(_current_key.key_code != _NONE)
    {
        if(_current_key.key_code == _prew_key.key_code)
        {
            _current_key.state = HOLD;
        }
        else
        {
            _current_key.state = PRESSED;
        }
    }
    else
    {
        _current_key.state = RELEASED;
    }
    
    _prew_key = _current_key;

    return _current_key;
}//getKey()
//------------------------------------------------------------------------------------------------------
inline uint8_t scanRows(void)
{
    uint8_t res = NUM_OF_ROWS;

    for(uint8_t i = 0; i < NUM_OF_ROWS; i++)
    {
        if(digitalRead(row[i]) == LOW)
            return i;
    }

    return res;
}//scanRows()
//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------

#endif //_KEYPAD_H