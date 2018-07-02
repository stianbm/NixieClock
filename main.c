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
#include <avr/interrupt.h>  //Atmel's own interrupt library


// Global constant
static unsigned int LED = 0b00010000;
static unsigned int TOSC1Pin = 0b00001000;

// Function declaration
void initializeLED();
void toggleLED();
void initializeCrystal();


int main(void)
{
	initializeLED();
	initializeCrystal();
	sei();
    while (1) 
    {
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
	//CLKCTRL.XOSC32KCTRLA |= 0b00110110;
	CLKCTRL.XOSC32KCTRLA |= 0b00110000;		// Start-up time of 64k cycles (longest), select external source type as external crystal
	//CLKCTRL.XOSC32KCTRLA |= 0b00000010;		// Runstdby - start the crystal
	CLKCTRL.XOSC32KCTRLA |= 0b00000001;		// Enable the oscillator
	// Write the clock select bits
	RTC.CLKSEL |= 0b00000010;				// Set pin 1 and 0 to 10 - 32.768 kHz from XOSC32K or external clock from TOSC1
	// Enable interrupt in peripheral
	RTC.PITINTCTRL |= 0B00000001;
	// Select period
	RTC.PITCTRLA |= 0b01111000;
	// Enable PIT
	RTC.PITCTRLA |= 0b00000001;
}


ISR(RTC_PIT_vect){
	toggleLED();
}


/* TODO:
 * Check the Busy bits in the RTC.STATUS and RTC.PITSTATUS registers, also on initial configuration
 */