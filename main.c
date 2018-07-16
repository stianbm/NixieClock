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
#include <avr/interrupt.h>  //Atmel's own interrupt library


// Global constant
static unsigned int LED = 0b00010000;

void toggleLED(){
	PORTB.OUT ^= LED;
}


void initializeLED(){
	// Set PB4/LED0 as output
	PORTB.DIR |= LED;
	// Set 1 as default value (off)
	PORTB.OUT |= LED;
}


void initializeCrystal(){
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


// Interrupt handler, resets interrupt flag uppon completion automatically
ISR(RTC_PIT_vect){
	toggleLED();
	RTC_PITINTFLAGS |= 0xFF;
}


int main(void){
	initializeLED();
	initializeCrystal();
	sei();
	while (1)
	{
	}
}