/*
 * FSK.c
 *
 * Created: 03/07/2018 23:46:38
 *  Author: Manuel
 */ 

#define F_CPU 14745600

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "FSK.h"


void init_FSK()
{
	DDRD |= (1<<PD5);
	TCCR1A = (1<<COM1A0);
	TCCR1B = FSK_ON;
}


void char_c_FSK(int tx_count)
{
	int i, j;
	int charax[8] = {1,1,0,0,0,0,1,0};
	//int charax[8] = {1,0,1,0,1,0,1,0};
		
	DDRD |= (1<<PD5);
	TCCR1A = (1<<COM1A0);
	TCCR1B = FSK_ON;
	
	for(j = 0; j < tx_count; j++) {
		TCNT1 = 0;
		while (i!=8) {
			DDRD |= (1<<PD5);
			// TCCR1B = FSK_ON;
			
			for (i=0;i<8;i++) {
				if (charax[i]==1) {
					//OCR1A = 115;//1khz is 115
					OCR1A = 7372;
					_delay_us(9091);
				}
				else{
					
					//OCR1A = 38;//3khz is 38
					OCR1A = 2457;
					_delay_us(9091);
				}
				
			}
			DDRD ^= (1<<PD5);
			//TCCR1B = FSK_OFF;
			
			_delay_ms(50);
		}
	}
}