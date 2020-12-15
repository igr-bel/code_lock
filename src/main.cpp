#include "functions.h"

//======================================================================================================
//------------------------------------------------------------------------------------------------------
//                                        SETUP
//------------------------------------------------------------------------------------------------------
void setup(void)
{
    //------------------------------------------------------ GAME INITS
    hardware_init();
    //------------------------------------------------------ phone keypad
    keypadInit();
    //------------------------------------------------------ sounds

}//setup()
//------------------------------------------------------------------------------------------------------
//                                        LOOP
//------------------------------------------------------------------------------------------------------
void loop(void)
{

  //------------------------------------------- ALGORITHM >>>
  //STATE MACHINE
    switch(current_state)
    {
		case MAIN_WORK_STATE:
			current_key = getKey();
			if(current_key.state != RELEASED)
			{
				is_input_started = true;
				beepSound(SHORT);
				work_timer = millis();
				input_code[code_counter] = current_key.key_code;
				code_counter++;
				while(current_key.state != RELEASED)
				{
					current_key = getKey();
				}
			}//if button pressed

			if(((millis() - work_timer) >= 1500) && is_input_started)
			{
				err_code = checkInputCode(input_code, right_input_code, code_counter);
				switch(err_code)
				{
					case ERR_1:
						current_state = WRONG_CODE_STATE;
						break;

					case PROG_MODE:
						prog_leds_timer = millis();
						for(uint8_t i = 0; i < 2; i++ )
						{
							beepSound(LONG);
							delay(1000);
						}
						code_length = 0;
						current_state = PROG_STATE;
						break;

					case MASTER_MODE:
						current_state = RIGHT_CODE_STATE;
						break;

					case RIGHT_CODE_MODE:
						current_state = RIGHT_CODE_STATE;
						break;
				}//switch

				is_input_started = false;
				code_counter = 0;
				for(uint8_t i = 0; i < MAX_INPUT_CODE_LENGTH; i++)
				{
					input_code[i] = 0xFF;
				}
			}// if time to check code

			break;

		case PROG_STATE:
			//prog mode leds flashing
			if((millis() - prog_leds_timer) >= 250)
			{
				prog_leds_timer = millis();
				if(prog_leds_flag)
				{
					ledControl(GREEN, ON);
					ledControl(BLUE, OFF);
					prog_leds_flag = false;
				}
				else
				{
					ledControl(GREEN, OFF);
					ledControl(BLUE, ON);
					prog_leds_flag = true;
				}//if_else
			}

			current_key = getKey();
			if(current_key.state != RELEASED)
			{
				beepSound(SHORT);
				if(current_key.key_code == _N)
				{
					//save new code to EEPROM
					EEPROM.update(code_length_addr, code_length);
					for(uint8_t i = 0; i < code_length; i++)
					{
						EEPROM.update(i + code_addr_offset, right_input_code[i]);
					}

					//exit prog mode
					for(uint8_t i = 0; i < 4; i++)
					{
						is_input_started = false;
						ledControl(GREEN, OFF);
						ledControl(BLUE, OFF);
						beepSound(SHORT);
						delay(50);
					}
					current_state = MAIN_WORK_STATE;
				}
				else
				{
					right_input_code[code_length] = current_key.key_code;
					code_length++;
				}

				// wait button release
				while(current_key.state != RELEASED)
				{
					current_key = getKey();
				}

			}//if button pressed
			break;

		case RIGHT_CODE_STATE:
			ledControl(GREEN, ON);
			beepSound(LONG);
			relayControl(UNLOCK);
			delay(3000);
			ledControl(GREEN, OFF);
			relayControl(LOCK);
			is_input_started = false;
			code_counter = 0;
			for(uint8_t i = 0; i < MAX_INPUT_CODE_LENGTH; i++)
			{
				input_code[i] = 0xFF;
			}
			current_state = MAIN_WORK_STATE;
			break;

		case WRONG_CODE_STATE:
			ledControl(BLUE, ON);
			for(uint8_t i = 0; i < 3; i++)
			{	
				beepSound(SHORT);
				delay(50);
			}
			delay(200);
			ledControl(BLUE, OFF);
				
			is_input_started = false;
			code_counter = 0;
			for(uint8_t i = 0; i < MAX_INPUT_CODE_LENGTH; i++)
			{
				input_code[i] = 0xFF;
			}

			current_state = MAIN_WORK_STATE;
			break;

        default:
            break;
    }//switch STATE MACHINE

  //------------------------------------------- <<< ALGORITHM


  //------------------------------------------------------------------------------------

    //---DBG info
    #ifdef DBG

    #endif
  
}//loop()