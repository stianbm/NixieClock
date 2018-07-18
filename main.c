/*
 * NixieClock.c
 *
 * Created: 12/06/2018 22:14:46
 * Author : stianbm
 *
 * This is a test program to toggle the LED, which is on PB4 - active low, using the button SW0 on PB5
 * Solved by handling interrupts from the button
 */ 

#include <avr/io.h>
#define F_CPU 3333333UL     //The internal clock is 20Mhz, but it can't run that fast with 3,3V so it is prescaled by a factor of 6 to 3333333
#include <util/delay.h>     //Delay Library
#include <avr/interrupt.h>  //Atmel's own interrupt library

// Global constant
static unsigned int LED = 0b00010000;
uint8_t modeButton = 0b00000001;

void toggleLED(){
	PORTB.OUT ^= LED;
}


void initializeLED(){
	PORTB.DIR |= LED;	// Set PB4/LED0 as output
	PORTB.OUT |= LED;	// Set 1 as default value (off)
}


void initializeButton(){
	PORTB.PIN1CTRL |= PORT_ISC_FALLING_gc;		// falling edge, this is preferred when the button is pulled up
}


int main(void)
{
	initializeLED();
	initializeButton();
	sei();  //turns on ISP(interrupt service routine).
	while (1)
	{
	}
}


// Interrupt handler for mode button
ISR(PORTB_PORT_vect){
	if(!(PORTB.IN & modeButton)){
		toggleLED();
	}
}