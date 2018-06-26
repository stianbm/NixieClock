/*
 * NixieClock.c
 *
 * Created: 12/06/2018 22:14:46
 * Author : stianbm
 *
 * This is a test program to toggle the LED, which is on PB4 - active low, using external crystal oscillator on PB2/TOSC2 and PB3/TOSC1
 * Crystal need to be physically attached to the kit, sacrificing UART
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
void initializeCrystal();


int main(void)
{
	initializeLED();
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


void initializeCrystal(){
	// Configure desired oscillator
	CLKCTRL.XOSC32KCTRLA |= 0b00110110;
	CLKCTRL.XOSC32KCTRLA |= 0b00110000;		// Start-up time of 64k cycles, select external source type as external crystal
	CLKCTRL.XOSC32KCTRLA |= 0b00000010;		// Start the crystal 
	// Write the clock select bits
	
	// TODO fix dis
	CLKCTRL.XOSC32KCTRLA &= 0b11111100;		// Set pin 1 and 0 to 00
	CLKCTRL.XOSC32KCTRLA |= 0b00000010;		// Set pin 1 and 0 to 10 - 32.768 kHz from XOSC32K or external clock from TOSC1
}