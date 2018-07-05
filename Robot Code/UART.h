/*
 * UART.h
 *
 * Created: 15/06/2018 21:09:57
 *  Author: Manuel
 */ 

/* 
 * Printing float values (ONLY NEEDS TO BE DONE ONCE PER PROJECT):
 *
 * Go to Project -> "Project" Properties -> Toolchain 
 * Under the AVR/GNU Linker -> Libraries, click on the add item option and addthe following libraries if they aren’t there already: "libscanf_flt" , "libprintf_flt", "libm".
 * Under AVR/GNU Linker -> General, set the option to "Use vprintf(-Wl,-u,vfprintf)"
 * Under AVR/GNU Linker -> Miscellaneous, inside of "Other Linker Flags", type "-Wl,-u,vfprintf"
 * Save (crtl+s) and exit the properties menu.
 * To print a float value, the following function needs to be used: prinft_P(PSTR("%f %d %c\n"), <float variable>, <int. var.>, <char. var.>)
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#ifndef UART_H_
#define UART_H_


int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);
void init_uart(void);


#endif /* UART_H_ */