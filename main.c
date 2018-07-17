/*
 * NixieClock.c
 *
 * Created: 12/06/2018 22:14:46
 * Author : stianbm
 *
 *
 * The Nixie clock is supposed to show four digits, two for hours and two for minutes.
 *
 * It also has two small Neon tubes to work as a colon, separating hours from minutes.
 *
 * Two buttons are used for setting the time, press one to enter the mode, hour digits
 * blink, minutes don't.
 *
 * Press other to increase hours, wraps around after 23.
 *
 * Press first to set minutes, minute digits blink.
 *
 * Press other to increase minutes, wrap around after 59.
 *
 * Press first to return to normal operation.
 *
 * Colon should toggle each second.
 *
 * Keep track of time in three variables: seconds, minutes and hours.
 * 
 *
 * Based on the work of JoergenSteen @github and Atmel's START projects
 */ 

#include <avr/io.h>								// A part of all AVR-programming for convenient programming
#include <avr/interrupt.h>						// Atmel's own interrupt library
#include "ConstantsAndVariables.h"				// Declarations of constants and variables used

#define F_CPU 3333333UL							// The internal clock is 20Mhz, but it can't run that fast with 3,3V so it is prescaled by a factor of 6 to 3333333


void initializeCrystalAndPIT(){
	// Deactivate change protection for register and start XOSC32K
	// Start-up time is set to the maximum 64K cycles (~2 seconds)
	// Use the macro "_PROTECTED_WRITE()"
	_PROTECTED_WRITE(CLKCTRL_XOSC32KCTRLA, CLKCTRL_ENABLE_bm | CLKCTRL_RUNSTDBY_bm | CLKCTRL_CSUT_64K_gc);
	// The RTC_STATUS needs to be 0 before writing to the RTC (could be used for start-up time).
	while (RTC.STATUS != 0) {}
	// Set crystal as clock source for RTC
	RTC_CLKSEL = RTC_CLKSEL_TOSC32K_gc;
	// Set the PIT periode to 32,768 cycles (1Hz) and enable PIT in RTC
	RTC_PITCTRLA |= 0b01110000 | RTC_PITEN_bm;
	// Enable PIT
	RTC_PITINTCTRL |= 0x01;
	
}

// Enable interrupts globally, set output pins, set interrupt conditions, link interrupts to proper function
void initializeGeneral(){
	// Set pins for digits, tubes and colon as output:
	PORTA.DIR = 0b11111110;
	PORTB.DIR = 0b11000001;
	PORTC.DIR = 0b00101111;
	
	/*

	Not used for buttons not on devboard?

	PORTB.PIN1CTRL |= (1<<PORT_PULLUPEN_bp);	// Turns on the pull-up resistor on PORTB1/button1
	PORTB.PIN1CTRL |= PORT_ISC_FALLING_gc;		// falling edge on PA5. This is preferred when the button is pulled up
	*/
	
	// Turn on ISP(interrupt service routine) globally
	sei();

	initializeCrystalAndPIT();
}

// Writes the digits to the tubes once per tube - might have to add delay later
void writeToTubes(){
	for(uint8_t i = 0; i<4; i++){
		// Clear and set tube chooser pin tubes[i]
		PORTC.OUT &= clearTubeSelect;
		PORTC.OUT |= tubes[i];
		// Set tube pin digits[i]
		if(digits[i] < 8 && digits[i] > 0){
			PORTA.OUT &= clearDigitPortA;	// Clear digits on port A
			PORTA.OUT |= (1<<digits[i]);
		}
		else{
			PORTB.OUT &= clearDigitPortB;	// Clear digits on port B
			switch(digits[i]){
				case 9:
					PORTB.OUT |= nine;
					break;
				case 8:
					PORTB.OUT |= eight;
					break;
				case 0:
					PORTB.OUT |= zero;
					break;
			}
		}
	}
}

// Splits hours into its two digits
void hoursToDigits(){
	hoursTemp = hours;
	digits[3] = 0;
	digits[2] = 0;
	
	while((hoursTemp - 10) >= 0){
		hoursTemp -= 10;
		digits[3]++;
	}
	digits[2] = hoursTemp;
}

void minutesToDigits(){
	minutesTemp = minutes;
	digits[1] = 0;
	digits[0] = 0;
	
	while((minutesTemp - 10) >= 0){
		minutesTemp -= 10;
		digits[1]++;
	}
	digits[0] = minutesTemp;
}

// Updates the digits
void updateDigits(){
	hoursToDigits();
	minutesToDigits();
}

// Convert the second counter to hours and minutes, then to digits and then display them
void updateLoop(){
	while(1){
		updateDigits();
		writeToTubes();
	}
}

void incrementHours(){
	if(hours >= maxHours){
		hours = 0;
	}
	else{
		hours++;
	}
}

void incrementMinutes(){
	if(minutes >= maxMinutes){
		minutes = 0;
		incrementHours();
	}
	else{
		minutes++;
	}
}

void toggleColon(){
	PORTC.OUT ^= colon;
}

// Increments the seconds counter, called by time interrupt, also toggle Neon tubes for colon
void incrementSeconds(){
	if(seconds >= maxSeconds){
		seconds = 0;
		incrementMinutes();
	}
	else{
		seconds++;
	}
	// Toggle the colon neon tubes
	toggleColon();
}

// Loop for blinking digits and changing values for hours and minutes
void setTimeLoop(){
	while(1){
		
	}
}

// Interrupt handler for PIT, either increments seconds or toggle, depending on modeFlags
ISR(RTC_PIT_vect){
	if(!modeFlags){
		incrementSeconds();
	}
	else{
		blinkDigits();
	}
	// Reset interrupt flag
	RTC_PITINTFLAGS |= 0xFF;
}

// Calls the initializer and starts the update loop
int main(void){
	initializeGeneral();
	updateLoop();
}


/* TODO:
 *
 * Make a loop for setting time - solve how to toggle hour tubes and minute tubes
 * Implement interrupt handlers for buttons
 * Give priority to PIT
 */

/* Pins and functions:
GND
VDD

PA0		Reserved
PA1		Digit 1
PA2		Digit 2
PA3		Digit 3
PA4		Digit 4
PA5		Digit 5
PA6		Digit 6
PA7		Digit 7

PB0		Digit 0
PB1		Button Mode
PB2		CRT
PB3		CRT
PB4		PWM
PB5		ADC
PB6		Digit 9
PB7		Digit 8

PC0		TubeChoser 1
PC1		TubeChoser 2
PC2		TubeChoser 3
PC3		TubeChoser 4
PC4		Button Value
PC5		Colon
*/