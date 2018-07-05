/*
 * Timer_Functions.c
 *
 * Created: 15/06/2018 22:34:57
 *  Author: Manuel
 */ 

#include <stdio.h>
#include <avr/io.h>

#include "Timer_Functions.h"

void delay_10ms(unsigned int delay_count)
{
	unsigned int i;

	for(i = 0; i < delay_count; i++) {
		TCCR1A = 0x00;					// Timer 0 is set for Normal Mode
		TCCR1B = (1<<CS12) | (1<<CS10);	// Pre-scaler is 1024
		OCR1A = 144;					// Number that we will compare TCNT1 to. 144*1024/F_CPU = 10 ms exactly
		TCNT1 = 0;						// Initializing TCNT0
		TIFR1 = (1<<OCF1A);				// Clears the Output Compare flag that will set when TCNT1 matches OCR1A
		while(!(TIFR1 & (1<<OCF1A)));	// Do nothing while the clock runs for 10ms
	}
	TCCR1B=0;	// Stop timer
}

void delay_1ms(unsigned int delay_count)
{
	unsigned int i;

	for(i = 0; i < delay_count; i++) {
		TCCR1A = 0x00;					// Timer 0 is set for Normal Mode
		TCCR1B = (1<<CS10);				// Pre-scaler is 1
		OCR1A = 14746;					// Number that we will compare TCNT1 to. 1843*1/F_CPU = 1.0000271267 ms
		TCNT1 = 0;						// Initializing TCNT0
		TIFR1 = (1<<OCF1A);				// Clears the Output Compare flag that will set when TCNT1 matches OCR1A
		while(!(TIFR1 & (1<<OCF1A)));	// Do nothing while the clock runs for 1ms
	}
	TCCR1B=0;	// Stop timer
}