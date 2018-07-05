/*
 * Motor_control.c
 *
 * Created: 04/07/2018 00:02:30
 *  Author: Manuel
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "Motor_control.h"
#include "Timer_Functions.h"

unsigned char left_motor_timer_A_reg_state;
unsigned char left_motor_timer_B_reg_state;
unsigned char right_motor_timer_A_reg_state;
unsigned char right_motor_timer_B_reg_state;
int right_ticks = 0; //count right wheel ticks
int left_ticks = 0; //count left wheel ticks
int tick_error = 0; //velocity error
float distance_mm = 0;
float left_velocity = 0;
float right_velocity = 0;
float error_angle = 0;

//void init_IR_sensors()
//{
	//DDRD &= (0<<PD4);
	//DDRA &= (0<<PA7);
	//
	//PCICR |= (1<<PCIE0)|(1<<PCIE3);  //pin change register,
	//PCIFR |= ( 1<< PCIF0)|(1<<PCIF3); // flag register
	//PCMSK0 = (1<< PCINT7);            // mask register
	//PCMSK3 = (1<< PCINT28);
//}

void init_IR_sensors()
{
	DDRD &= 0xF3;
	PORTD |= (1<<PD2)|(1<<PD3);
	
	EIMSK = (1<<INT0)|(1<<INT1);
	EIFR = (1<<INTF0)|(1<<INTF1);
	EICRA = (1<<ISC21);
}

void init_motors()
{
	DDRB |= (1<<PB4)|(1<<PB3);
	DDRB &= (0<<PB0); //PB0 is left sensor input, PB3 and PB4 are PWM out for left motor
	PORTB |= (1<<PB4)|(1<<PB3);
	DDRC &= (0<<PC0); //PC0 is the right sensor input
	DDRD |= (1<<PD7)|(1<<PD6); //PD6 and PD7 are outputs for the right motor
	PORTD |= (1<<PD7)|(1<<PD6);
		
	//this is setting up PB0 and PC0 as the input interrupts for the sensors. The interrupt ISR is implemented in the main file.
	PCICR |= (1<<PCIE1)|(1<<PCIE2);
	PCMSK1 |= (1<<PCINT8); //this corresponds to PORTB pin 0
	PCMSK2 |= (1<<PCINT16);//this corresponds to PORTC pin 0
}


void error_analysis()
{
	left_velocity = (left_ticks * DISTANCE_COEFF) / (SAMPLE_TIME_IN_10MS * 0.01);
	right_velocity = (right_ticks * DISTANCE_COEFF) / (SAMPLE_TIME_IN_10MS * 0.01);
	error_angle += (right_ticks - left_ticks) * DISTANCE_COEFF / (WHEEL_SEPARATION_LENGTH);
	
	printf_P(PSTR("%5.3f %d	 %d	 %d	 %d	 %5.3f\n"),distance_mm, OCR0A, left_ticks, OCR2B, right_ticks, error_angle*RADS_2_DEG_CONV);
	
	//printf_P(PSTR("Left Velocity | Right Velocity | Resulting angle error\n"));
	//printf_P(PSTR("   %5.3f      |       %5.3f    |           %5.3f\n"), left_velocity, right_velocity, error_angle*RADS_2_DEG_CONV);
	
	//printf_P(PSTR("Iteration | Tick error | Left-tick   left-OCR-value | Right-tick right-OCR-value\n"));
	//printf_P(PSTR("    %d    |     %d     |     %d           %d        |     %d           %d\n"), sample_count, tick_error, left_ticks, OCR0A, right_ticks, OCR2A);
}


void Control()
{
	delay_10ms(SAMPLE_TIME_IN_10MS); //delay to accumulate data
	
	cli();
	tick_error = left_ticks - right_ticks; //+ve error, speed right up, -ve error slow right down
	distance_mm += ((float)(left_ticks + right_ticks))*DISTANCE_COEFF/2;
	
	if(tick_error > 0) { //this is speed up
		OCR2A-=1;
		OCR2B-=1;
	}
	else if(tick_error < 0) { //this is slow down
		OCR2A+=1;
		OCR2B+=1;
	}
	else {
		OCR2A = OCR2A;
		OCR2B = OCR2B;
	}
	
	//error_analysis();
	
	//reset Left and Right wheel ticks
	left_ticks = 0;
	right_ticks = 0;
	
	sei();
}

void left_motor_forward_set_up()
{
	DDRB |= (1<<PB4)|(1<<PB3)|(0<<PB0); //PB0 is left sensor input, PB3 and PB4 are PWM out for left motor
	PORTB |= (1<<PB4)|(1<<PB3);
	
	TCCR0A=(1<<COM0A1)|(1<<WGM01)|(1<<WGM00);
	TCCR0B=(1<<CS01);
	TCNT0=0;
	
	left_motor_timer_A_reg_state = TCCR0A;
	left_motor_timer_B_reg_state = TCCR0B;
	
	//PORTB |= (1<<PB3)|(1<<PB4);
	PORTB &= 0xE7;
}

void left_motor_reverse_set_up()
{
	DDRB |= (1<<PB4)|(1<<PB3)|(0<<PB0); //PB0 is left sensor input, PB3 and PB4 are PWM out for left motor
	//PORTB |= (1<<PB4)|(1<<PB3);
	
	TCCR0A=(1<<COM0A1)|(1<<COM0B0)|(1<<COM0B1)|(1<<WGM01)|(1<<WGM00);
	TCCR0B=(1<<CS01);
	TCNT0=0;
	
	left_motor_timer_A_reg_state = TCCR0A;
	left_motor_timer_B_reg_state = TCCR0B;
	
	//PORTB |= (1<<PB3)|(1<<PB4);
	PORTB &= 0xE7;
}

void left_motor_break()
{
	PORTB |= (1<<PB3);
	PORTB &= 0xEF;
	TCCR0A = 0x00;
	TCCR0B = 0x00;
	
	//left_motor_timer_A_reg_state = TCCR0A;
	//left_motor_timer_B_reg_state = TCCR0B;
}

void right_motor_forward_set_up()
{
	DDRC |= (0<<PC0); //PC0 is the right sensor input
	DDRD |= (1<<PD7)|(1<<PD6); //PD6 and PD7 are outputs for the right motor
	//PORTD |= (1<<PD7)|(1<<PD6);
	
	TCCR2A=(1<<COM2B1)|(1<<WGM21)|(1<<WGM20);
	TCCR2B=(1<<CS21);
	TCNT2=0;
	
	right_motor_timer_A_reg_state = TCCR2A;
	right_motor_timer_B_reg_state = TCCR2B;
	
	//PORTD |= (1<<PD7)|(1<<PD6);
	PORTD &= 0x3F;
}

void right_motor_reverse_set_up()
{
	DDRC |= (0<<PC0); //PC0 is the right sensor input
	DDRD |= (1<<PD7)|(1<<PD6); //PD6 and PD7 are outputs for the right motor
	PORTD |= (1<<PD7)|(1<<PD6);
	
	TCCR2A=(1<<COM2A1)|(1<<COM2A0)|(1<<COM2B1)|(1<<WGM21)|(1<<WGM20);
	TCCR2B=(1<<CS21);
	TCNT2=0;
	
	right_motor_timer_A_reg_state = TCCR2A;
	right_motor_timer_B_reg_state = TCCR2B;
	
	//PORTD |= (1<<PD7)|(1<<PD6);
	PORTD &= 0x3F;
}

void right_motor_break()
{
	PORTD |= (1<<PD6);
	PORTD &= 0x7F;
	TCCR2A = 0x00;
	TCCR2B = 0x00;
	
	//right_motor_timer_A_reg_state = TCCR2A;
	//right_motor_timer_B_reg_state = TCCR2B;
}

void forward(float distance_threshold)
{
	left_ticks = 0;
	right_ticks = 0;
	
	left_motor_forward_set_up();
	right_motor_forward_set_up();
	
	//OCRxA at 64 equals a 7.2kHz square wave with a 25.4% duty cycle
	OCR0A = LEFT_MOTOR_DEFAULT_DUTY_CYCLE; //PB3 output
	OCR0B = LEFT_MOTOR_DEFAULT_DUTY_CYCLE; //PB4 output
	
	OCR2A = RIGHT_MOTOR_DEFAULT_DUTY_CYCLE; //PD7 output
	OCR2B = RIGHT_MOTOR_DEFAULT_DUTY_CYCLE; //PD6 output
	
	while (distance_mm < distance_threshold)
	{
		Control();
	}
	
	left_motor_break();
	right_motor_break();
	
	distance_mm = 0;
}

void reverse(int run_time_10ms)
{
	left_ticks = 0;
	right_ticks = 0;
	
	unsigned char run_complete = 0;
	
	left_motor_reverse_set_up();
	right_motor_reverse_set_up();
	
	OCR0A = LEFT_MOTOR_DEFAULT_DUTY_CYCLE; //PB3 output
	OCR0B = LEFT_MOTOR_DEFAULT_DUTY_CYCLE; //PB4 output
	
	OCR2A = RIGHT_MOTOR_DEFAULT_DUTY_CYCLE; //PD7 output
	OCR2B = RIGHT_MOTOR_DEFAULT_DUTY_CYCLE; //PD6 output
	
	while (!run_complete)
	{
		Control();
		delay_10ms(run_time_10ms);
		run_complete = 1;
	}
	
	left_motor_break();
	right_motor_break();
}

void forward_right_turn()
{
	left_ticks = 0;
	right_ticks = 0;
	
	right_motor_break();
	left_motor_forward_set_up();

	OCR0A = LEFT_MOTOR_DEFAULT_DUTY_CYCLE; //PB3 output
	OCR0B = LEFT_MOTOR_DEFAULT_DUTY_CYCLE; //PB4 output
	
	while (left_ticks <= _90DEG_TURN_TICKS) {
		delay_10ms(1);
	}
	
	left_motor_break();
	right_motor_break();
}

void forward_left_turn()
{
	left_ticks = 0;
	right_ticks = 0;
	
	left_motor_break();
	right_motor_forward_set_up();
	
	OCR2A = RIGHT_MOTOR_DEFAULT_DUTY_CYCLE; //PD7 output
	OCR2B = RIGHT_MOTOR_DEFAULT_DUTY_CYCLE; //PD6 output
	
	while (right_ticks <= _90DEG_TURN_TICKS){
		delay_10ms(1);
	}
	
	right_motor_break();
}

void reverse_left_turn()
{
	left_ticks = 0;
	right_ticks = 0;
	
	right_motor_break();
	left_motor_reverse_set_up();
	
	OCR0A = LEFT_MOTOR_DEFAULT_DUTY_CYCLE; //PB3 output
	OCR0B = LEFT_MOTOR_DEFAULT_DUTY_CYCLE; //PB4 output
	
	while (left_ticks <= _90DEG_TURN_TICKS){
		delay_10ms(1);
	}
	
	left_motor_break();
}

void reverse_right_turn()
{
	left_ticks = 0;
	right_ticks = 0;
	
	left_motor_break();
	right_motor_reverse_set_up();
	
	OCR2A = LEFT_MOTOR_DEFAULT_DUTY_CYCLE; //PB3 output
	OCR2B = LEFT_MOTOR_DEFAULT_DUTY_CYCLE; //PB4 output
	
	while (right_ticks <= _90DEG_TURN_TICKS){
		delay_10ms(1);
	}
	
	right_motor_break();
}