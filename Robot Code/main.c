/*
 * Robot Code.c
 *
 * Created: 15/06/2018 21:03:50
 * Author : Manuel
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "UART.h"
#include "Timer_Functions.h"
#include "FSK.h"
#include "Motor_control.h"

ISR(PCINT1_vect) //this corresponds to PORTB Pin 0
{
	left_ticks++;
}


//pin change interrupt, (RIGHT WHEEL PC0)
ISR(PCINT2_vect) //this corresponds to PORTC Pin 0
{
	right_ticks++;
}

ISR(INT0_vect)
{
	right_motor_break();
	delay_10ms(10);
	TCCR2A = right_motor_timer_A_reg_state;
	TCCR2B = right_motor_timer_B_reg_state;
	PORTD &= 0x3F;
}

ISR(INT1_vect)
{	
	left_motor_break();
	delay_10ms(10);
	TCCR0A = left_motor_timer_A_reg_state;
	TCCR0B = left_motor_timer_B_reg_state;
	PORTB &= 0xE7;
}

int main(void)
{
	init_uart();
	init_IR_sensors();
    init_motors();
	
	//DDRB=0x18; //PB0 is left sensor input, PB3 and PB4 are PWM out for left motor
	//DDRC=0x00; //PC0 is the right sensor input
	//DDRD=0xF0; //PD6 and PD7 are outputs for the right motor
	//PORTB = 0x18;
	//PORTD = 0xc0;
	//
	////this is setting up PB0 and PC0 as the input interrupts for the sensors
	//PCICR=(1<<PCIE1)|(1<<PCIE2);
	//PCMSK1=(1<<PCINT8); //this corresponds to PORTB pin 0
	//PCMSK2=(1<<PCINT16);//this corresponds to PORTC pin 0
	
    while (1) 
    {
		delay_10ms(200);
		//reverse(200);
		//forward(500);
		forward_left_turn();
		//forward(1000);
		//forward_left_turn();
		//forward(1000);
		//char_c_FSK(10);
		//reverse(50);
		//forward_left_turn();
		//reverse_left_turn();
		//forward_right_turn();
		//reverse_right_turn();
		//delay_10ms(200);
    }
}

