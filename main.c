/*
 * NixieClock.c
 *
 * Created: 12/06/2018 22:14:46
 * Author : stianbm
 *
 * This is a test program to toggle the LED, which is on PB4 - active low, using the button SW0 on PB5
 * Solved by polling the button value
 */ 

#include <avr/io.h>
#define F_CPU 3333333UL     //The internal clock is 20Mhz, but it can't run that fast with 3,3V so it is prescaled by a factor of 6 to 3333333
#include <util/delay.h>     //Delay Library

// Global constant
static unsigned int LED = 0b00010000;
static unsigned int button = 0b00100000;

// Function declaration
void initializeLED();
void toggleLED();
void initializeButton();


int main(void)
{
	initializeLED();
	initializeButton();
    while (1) 
    {
		if(!(PORTB.IN & button)){
			toggleLED();
			_delay_ms(500); //Waits for 500ms
		}
    }
}


void toggleLED(){
	PORTB.OUT ^= LED;
}


void initializeLED(){
	PORTB.DIR |= LED;	// Set PB4/LED0 as output
	PORTB.OUT |= LED;	// Set 1 as default value (off)
}


void initializeButton(){
	PORTB.PIN5CTRL |= (1<<3);  //Turns on the pull-up resistor on PORTB5
}