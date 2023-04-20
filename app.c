/*
 * app.c
 *
 *  Created on: Mar 27, 2023
 *      Author: Ahmed Abdalla
 */
#include <util/delay.h>
#include <math.h>
#include "../MCAL/DIO/DIO_Interface.h"
#include "../HAL/LCD/lcd_interface.h"
#include "../HAL/LCD/lcd_cfg.h"
#include "../HAL/7_segment/7_segment.h"
#include "../HAL/Keypad/keypad_interface.h"
#include "../HAL/button/button_interface.h"
#include "../HAL/DC_Motor/DC_Motor_Interface.h"
#include "../HAL/Stepper_Motor/Stepper_motor_Interface.h"
#include "../MCAL/EXTI/EXTI_Interface.h"
#include "../MCAL/ADC/ADC_Interface.h"
#include "../MCAL/Timers/Timers_Interface.h"

#define GIE_ENABLE() (*((volatile u8 *)0x5F)) = 0x80

u16 keypad_u8_read_num(u16 *op);
u8 app_u16_Get_Pass();


u16 entered_pass = 0;
u16 num1 = 0 , num2 = 0;
u16 op = 1;
u16 pre_op = 0;
u8 no_of_operand = 0;
u16 result = 0;


u8 temp = 0;
u8 system_state = 0;
u8 motor_select = 0;
u8 DC_MOTOR_state = 0;
u8 Stepper_motor_option = 0;
u8 Stepper_motor_dir = 0;
u16 Stepper_motor_angle = 0;

s32 main()
{
	lcd_void_lcd_init();
	keypad_void_init();
	DC_MOTOR_Init();
	Stepper_MOTOR_Init();
	DIO_void_Pin_Initialize(PORTB, PIN2, OUTPUT);

system_state = app_u16_Get_Pass();
	while (1)
	{

		if(44 ==system_state )
		{
			op = 0;
			lcd_void_send_command(_LCD_CLEAR);
			//lcd_void_send_command(0x14);
			lcd_void_send_str_pos("ahlan" , ROW_1 , 1);
			_delay_ms(1000);
			lcd_void_send_command(_LCD_CLEAR);
			lcd_void_send_str_pos("1)DC Motor" , ROW_1 , 1);
			lcd_void_send_str_pos("2)step Motor" , ROW_2 , 1);
			//lcd_void_send_str_pos("3)Ret" , ROW_1 , 12);

			//_delay_ms(3000);
			while(1)
			{
				motor_select = keypad_u8_read_num(&op);
				if( 1 == motor_select)
				{
					lcd_void_send_command(_LCD_CLEAR);
					lcd_void_send_str_pos("1)Clock" , ROW_1 , 1);
					lcd_void_send_str_pos("2)Anti CLock" , ROW_2 , 1);
					lcd_void_send_str_pos("3)Ret" , ROW_1 , 12);

					//DC_MOTOR_state = keypad_u8_read_num(&op);
					while(1)
					{
						lcd_void_send_str_pos("1)Clock" , ROW_1 , 1);
						lcd_void_send_str_pos("2)Anti CLock" , ROW_2 , 1);
						lcd_void_send_str_pos("3)Ret" , ROW_1 , 12);
						DC_MOTOR_state = keypad_u8_read_num(&op);
						if(1 == DC_MOTOR_state)
						{
							DC_MOTOR_TURN_ON(0x00);
						}
						else if(2 == DC_MOTOR_state)
						{
							DC_MOTOR_TURN_ON(0x01);
						}
						else if(3 == DC_MOTOR_state)
						{
							DC_MOTOR_TURN_OFF();
							break;
						}
					}

					if(3 == DC_MOTOR_state)
					{
						DC_MOTOR_TURN_OFF();
						break;
					}
				}
				else if(2 == motor_select)
				{
					//Stepper motor
					while(1)
					{
						lcd_void_send_command(_LCD_CLEAR);
						lcd_void_send_str_pos("1)Direction" , ROW_1 , 1);
						lcd_void_send_str_pos("2)Angle" , ROW_2 , 1);
						lcd_void_send_str_pos("3)Ret" , ROW_1 , 12);

						Stepper_motor_option = keypad_u8_read_num(&op);
						if(1 == Stepper_motor_option)
						{
							while(1)
							{
								lcd_void_send_command(_LCD_CLEAR);
								lcd_void_send_str_pos("1)Clock" , ROW_1 , 1);
								lcd_void_send_str_pos("2)Anti clock" , ROW_2 , 1);
								lcd_void_send_str_pos("3)Ret" , ROW_1 , 12);

								//Stepper_motor_dir = keypad_u8_read_num(&op);
								while(1)
								{

									Stepper_motor_dir = keypad_u8_read_num(&op);
									if(1 == Stepper_motor_dir)
									{
										lcd_void_send_str_pos("OK" , ROW_2 , 15);
										temp = 0x01;
										//Stepper_MOTOR_TURN_ON(0x00 , 0);
									}
									else if(2 ==Stepper_motor_dir )
									{

										temp = 0x00;
										lcd_void_send_str_pos("OK" , ROW_2 , 15);
										//Stepper_MOTOR_TURN_ON(0x01 ,0);
									}
									else if(3 ==Stepper_motor_dir)
									{
										//lcd_void_send_char_pos('&', ROW_2, 15);
										break;
									}
								}
								if(3 ==Stepper_motor_dir)
								{
									//lcd_void_send_char_pos('$', ROW_2, 14);
										break;
								}
							}
//							if(3 ==Stepper_motor_dir)
//							{
//								lcd_void_send_char_pos('#', ROW_2, 13);
//									break;
//							}

						}
						else if(2 == Stepper_motor_option)
						{
							while(1)
							{
								lcd_void_send_command(_LCD_CLEAR);
								lcd_void_send_str_pos("Enter angle" , ROW_1 , 1);
								lcd_void_send_char_pos('>', ROW_2, 1);
								Stepper_motor_angle = keypad_u8_read_num(&op);
								if(Stepper_motor_angle >360)
								{
									lcd_void_send_str_pos("INVALID angle" , ROW_1 , 1);
									_delay_ms(1000);
									lcd_void_send_str_pos("Enter angle" , ROW_1 , 1);
									lcd_void_send_char_pos('>', ROW_2, 1);

									Stepper_motor_angle = keypad_u8_read_num(&op);
								}
								else
								{
									lcd_void_send_num_pos(Stepper_motor_angle, ROW_2, 15);
									Stepper_MOTOR_TURN_ON(temp ,Stepper_motor_angle );
									break;
								}
							}
						}
						else if(3 == Stepper_motor_option)
						{
							break;
						}
					}
					if(3 == Stepper_motor_option)
					{
						Stepper_MOTOR_TURN_OFF();
						break;
					}
				}

			}

		}
		else if(33 == system_state)
		{
			DIO_void_Pin_Write_Logic(PORTB, PIN2, HIGH);
			lcd_void_send_str_pos("Restart Me" , ROW_1 , 1);
			_delay_ms(1000);
			break;
		}

	}
	return 0;
}

u8 app_u16_Get_Pass()
{
	u8 status = 55;
	u16 ret_pass = 1234;
	lcd_void_send_command(_LCD_CLEAR);
	lcd_void_send_str_pos("Welcome ya User" , ROW_1 , 1);
	_delay_ms(500);
	lcd_void_send_command(_LCD_CLEAR);
	lcd_void_send_str_pos("Enter Password" , ROW_1 , 1);
	lcd_void_send_command( (0xc0));
	lcd_void_send_str_pos("Enter Password" , ROW_1 , 1);
	lcd_void_send_command(_LCD_DISPLAY_ON_CURSOR_ON_CHAR_BLINK_ON);

	for(u8 counter = 0; counter <3 ; counter++)
	{
		entered_pass = keypad_u8_read_num(&op);
		if(entered_pass == 1234)
		{
			status = 44;
			lcd_void_send_command(_LCD_DISPLAY_ON_CURSOR_OFF_CHAR_BLINK_OFF);
			break;
		}
		else
		{
			lcd_void_send_command(_LCD_CLEAR);
			lcd_void_send_command(_LCD_DISPLAY_ON_CURSOR_OFF_CHAR_BLINK_OFF);
			lcd_void_send_str_pos("Wrong" , ROW_1 , 1);
			_delay_ms(300);
			lcd_void_send_command(_LCD_CLEAR);
			lcd_void_send_str_pos("Enter Password" , ROW_1 , 1);
			lcd_void_send_command( (0xc0));
			lcd_void_send_command(_LCD_DISPLAY_ON_CURSOR_ON_CHAR_BLINK_ON);

		}
		if(counter == 2)
		{
			lcd_void_send_command(_LCD_DISPLAY_ON_CURSOR_OFF_CHAR_BLINK_OFF);
			status = 33;
			lcd_void_send_command(_LCD_CLEAR);
			lcd_void_send_str_pos("7ramy" , ROW_1 , 1);
			_delay_ms(3000);
		}
	}
	op=0;
	return status;
}


//=============================================================
u16 keypad_u8_read_num(u16 *op)
{
	//wasted a lot of time in debugging because these local variable weren't initialized
	u8 digit = 0;
	u16 num = 0;
	u16 power = 1;
	u8 arr1[10] = {0};
	u8 arr_1_counter = 0;
	u32 local_btn_counter=0;
	//digit = keypad_u8_read_digit();

	while (1)
	{
		digit = keypad_u8_read_digit();

		if(((digit <=9) || (digit ==0) || (digit == 77)) && (digit !=0xff) )
		{
			if((digit == 77))
			{
				arr_1_counter--;
				lcd_void_send_char_pos(' ', ROW_2, arr_1_counter +1);
				lcd_void_send_command((0xc0) +arr_1_counter +1 );
			}
			else
			{
				arr1[arr_1_counter] = digit;

				if(*op == 1)
				{
					lcd_void_send_char_pos('*', ROW_2, arr_1_counter +1);
					//lcd_void_send_char('*');
				}
				else if(*op == 0)
				{
					lcd_void_send_num_pos(arr1[arr_1_counter], ROW_2, arr_1_counter +1);
					//lcd_void_send_num(arr1[arr_1_counter]);
				}
				arr_1_counter++;
				//*op = 0;
			}

		}
		else if((digit == 11) || (digit == 22)|| (digit == 33)|| (digit == 44)|| (digit == 99))
		{
			for(int i =0 ; i<arr_1_counter ; i++)
			{
				for(u8 j = 1 ; j <(arr_1_counter -i ) ; j++)
				{
					power = power *10;
				}
				num += arr1[i] *power;
				power = 1;
			}
			*op = digit;

			break;
		}
	}
	return num;
}
