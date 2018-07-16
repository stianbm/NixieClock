/*
 * NixieClock.c
 *
 * Created: 12/06/2018 22:14:46
 * Author : stianbm
 *
 * This is a test program to count seconds from zero to ten on a seven element thingy
 * Crystal need to be physically attached to the kit, sacrificing UART
 */ 

#include <avr/io.h>
#define F_CPU 3333333UL     //The internal clock is 20Mhz, but it can't run that fast with 3,3V so it is prescaled by a factor of 6 to 3333333
#include <avr/interrupt.h>  //Atmel's own interrupt library


/*
 * 7-segment display
 *			a
 *	      _ _ _    
 *		|	    |
 *	  f	|       | b
 *		|		|
 *		  _ _ _ 
 *		|	g   |
 *	  e	|       | c
 *      |       |
 *        _ _ _ 
 *          d
 */ 


// Global variables
// Port A
static uint8_t dot = 0b00000010;
static uint8_t d = 0b000000100;
static uint8_t c = 0b10000000;
static uint8_t g = 0b01000000;
// Port C
static uint8_t a = 0b00000001;
static uint8_t b = 0b00001000;
static uint8_t e = 0b00000100;
static uint8_t f = 0b00000010;
// "Reset" segments
static uint8_t ABlank = 0b00111001;
static uint8_t CBlank = 0b11110000;
// Temps
static uint8_t PortATemp = 0;
static uint8_t PortCTemp = 0;

uint8_t sec = 0;



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


void portInit(){
	PORTA.DIR |= ~ABlank;
	PORTC.DIR |= ~CBlank;
}


void wipe(){
	PORTA.OUT &= ABlank;
	PORTC.OUT &= CBlank;

	PortATemp = 0;
	PortCTemp = 0;
}


void writeDigit(uint8_t digit){
	wipe();
	switch(digit){
		case 0:
			PortATemp = (c|d);
			PortCTemp = (a|b|e|f);
			break;
		case 1:
			PortATemp = c;
			PortCTemp = b;
			break;
		case 2:
			PortATemp = (d|g);
			PortCTemp = (a|b|e);
			break;
		case 3:
			PortATemp = (c|d|g);
			PortCTemp = (a|b);
			break;
		case 4:
			PortATemp = (c|g);
			PortCTemp = (b|f);
			break;
		case 5:
			PortATemp = (c|d|g);
			PortCTemp = (a|f);
			break;
		case 6:
			PortATemp = (c|d|g);
			PortCTemp = (e|f);
			break;
		case 7:
			PortATemp = c;
			PortCTemp = (a|b);
			break;
		case 8:
			PortATemp = (c|d|g);
			PortCTemp = (a|b|e|f);
			break;
		case 9:
			PortATemp = (c|g);
			PortCTemp = (a|b|f);
			break;
	}
	PORTA.OUT |= PortATemp;
	PORTC.OUT |= PortCTemp;
}


// Interrupt handler
ISR(RTC_PIT_vect){
	if(sec >= 9){
		sec = 0; 
	}
	else{
		sec++;
	}
	// Reset interrupt flag
	RTC_PITINTFLAGS |= 0xFF;
}


int main(void){
	portInit();
	initializeCrystal();
	sei();
	writeDigit(0);
	while (1)
	{
		writeDigit(sec);
	}
}