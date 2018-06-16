/*
 * NixieClock.c
 *
 * Created: 12/06/2018 22:14:46
 * Author : stianbm
 *
 * This is a test program to turn on the LED, which is on PORTB PIN4 - active low
 */ 

#include <avr/io.h>


int main(void)
{
	static unsigned int LED = 0b00010000;
	PORTB.DIR |= LED;
	PORTB.OUT &= ~LED;
    while (1) 
    {
    }
}

