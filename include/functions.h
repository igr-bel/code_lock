#ifndef FUNCTIONS_INCLUDED
#define FUNCTIONS_INCLUDED

#include <Arduino.h>
#include <EEPROM.h>
#include "keypad.h"
#include "main.h"

//------------------------------------------------------------------------------------------------------
//                                        DEFINES
//------------------------------------------------------------------------------------------------------
// #define DBG                             
#define NOP                             do{}while(0)

//GPIO
#define GREEN_LED_PIN					1
#define BLUE_LED_PIN					4

#define BUZZER_PIN						10

#define RELAY_PIN						A3

//------------------------------------------------------------------------------------------------------
//                                        OBJECTS
//------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------
//                                        GLOBAL VARIABLES
//------------------------------------------------------------------------------------------------------
enum
{
	LONG,
	SHORT
};

enum
{
	ON,
	OFF
};

enum
{
	LOCK,
	UNLOCK
};

enum
{
	GREEN,
	BLUE
};

enum
{
	NO_ERR,
	ERR_1,
	PROG_MODE,
	MASTER_MODE,
	RIGHT_CODE_MODE
};

uint8_t err_code = 0;

//------------------------------------------------------------------------------------------------------
//                                        FLAGS
//------------------------------------------------------------------------------------------------------
bool is_input_started = false;

//------------------------------------------------------------------------------------------------------
//                                        PROTOTYPES
//------------------------------------------------------------------------------------------------------
//HARDWARE
inline void hardware_init(void);

//WORK
void beepSound(uint8_t length);
void ledControl(uint8_t color, uint8_t state);
void relayControl(uint8_t state);
uint8_t checkInputCode(uint8_t * input_arr, uint8_t * control_arr, uint8_t length);

//RESET
void(* resetFunc) (void) = 0;

//------------------------------------------------------------------------------------------------------
//                                       BEHAVIOR
//------------------------------------------------------------------------------------------------------
inline void hardware_init(void)
{
    #ifdef DBG
        Serial.begin(9600);
        Serial.println("___DBG___");
        Serial.println("[DBG] start init ... ");
    #endif

    //------------------------------------------------------ GPIO
	pinMode(GREEN_LED_PIN, OUTPUT);
	digitalWrite(GREEN_LED_PIN, HIGH);
	pinMode(BLUE_LED_PIN, OUTPUT);
	digitalWrite(BLUE_LED_PIN, HIGH);

	pinMode(BUZZER_PIN, OUTPUT);
	digitalWrite(BUZZER_PIN, HIGH);

	pinMode(RELAY_PIN, OUTPUT);
	digitalWrite(RELAY_PIN, LOW);

    //------------------------------------------------------ START INITS
	current_state = MAIN_WORK_STATE;
	is_input_started = false;

	ledControl(GREEN, ON);
	code_length = EEPROM.read(code_length_addr);
	if(code_length > 0 && code_length < MAX_INPUT_CODE_LENGTH)
	{
		for(uint8_t i = 0; i < code_length; i++)
		{
			right_input_code[i] = EEPROM.read(i + code_addr_offset);
		}
	}
	ledControl(GREEN, OFF);

    #ifdef DBG
        Serial.println("[DBG] done!");
    #endif
}//hardware_init()
//------------------------------------------------------------------------------------------------------
void beepSound(uint8_t length)
{
	switch (length)
	{
	case LONG:
		analogWrite(BUZZER_PIN, 200);
		delay(1000);
		digitalWrite(BUZZER_PIN, HIGH);
		break;

	case SHORT:
		analogWrite(BUZZER_PIN, 200);
		delay(200);
		digitalWrite(BUZZER_PIN, HIGH);
		break;
	
	default:
		break;
	}

	return;
}//beepSound()
//------------------------------------------------------------------------------------------------------
void ledControl(uint8_t color, uint8_t state)
{
	switch (color)
	{
		case GREEN:
			digitalWrite(GREEN_LED_PIN, state);
			break;
		
		case BLUE:
			digitalWrite(BLUE_LED_PIN, state);
			break;

		default:
			break;
	}

	return;
}//ledControl()
//------------------------------------------------------------------------------------------------------
void relayControl(uint8_t state)
{
	digitalWrite(RELAY_PIN, state);

	return;
}//relayControl()
//------------------------------------------------------------------------------------------------------
uint8_t checkInputCode(uint8_t * input_arr, uint8_t * control_arr, uint8_t length)
{
	bool _error;

	//check length of input code
	if(length > MAX_INPUT_CODE_LENGTH)
	{
		return ERR_1;
	}

	// if first simbol is '*'
	if(input_arr[0] == _S)
	{
		// check prog mode
		_error = false;
		for(uint8_t i = 0; i < PROG_MODE_LENGTH; i++)
		{
			if(input_arr[i] != prog_mode_code[i])
			{
				_error = true;
			}
		}
		if(!_error)
		{
			return PROG_MODE;
		}

		//check master mode 
		_error = false;
		for(uint8_t i = 0; i < MASTER_MODE_LENGTH; i++)
		{
			if(input_arr[i] != master_code[i])
			{
				_error = true;
			}
		}
		if(!_error)
		{
			return MASTER_MODE;
		}
	}// if first simbol is '*'

	// check other codes
	if(length != code_length)
	{
		return ERR_1;
	}

	_error = false;
	for(uint8_t i = 0; i < length; i++)
	{
		if(input_arr[i] != control_arr[i])
		{
			_error = true;
		}
	}
	if(_error)
	{
		return ERR_1;
	}
	else
	{
		return RIGHT_CODE_MODE;
	}

}//checkInputCode()
//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------



#endif //FUNCTIONS_INCLUDED