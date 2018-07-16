/*
 * NixieClock.c
 *
 * Created: 12/06/2018 22:14:46
 * Author : stianbm
 *
 * The Nixie clock is supposed to show four digits, two for hours and two for minutes.
 * It also has two small Neon tubes to work as a colon, separating hours from minutes.
 * Two buttons are used for setting the time, press one to enter the mode, hour digits
 * blink, minutes don't.
 * Press other to increase hours, wraps around after 23.
 * Press first to set minutes, minute digits blink.
 * Press other to increase minutes, wrap around after	59.
 * Press first to return to normal operation.
 * Colon should toggle each second.
 * 
 * Based on the work of JoergenSteen @github and Atmel's START projects
 */ 

#include <avr/io.h>								// A part of all AVR-programming for convenient programming
#include <avr/interrupt.h>						// Atmel's own interrupt library

#define F_CPU 3333333UL							// The internal clock is 20Mhz, but it can't run that fast with 3,3V so it is prescaled by a factor of 6 to 3333333


// Global variables
unsigned long int seconds = 0;					// The main counter
static unsigned long int maxSeconds = 86439; 	// 24 hours in seconds - 1		Have to use 32b
static unsigned int secondsInHour = 3600;		// Seconds in an hour
static unsigned int secondsInMinute = 60;		// Seconds in a minute

unsigned int hours = 0;							// Counter for hours
unsigned int minutes = 0;						// Counter for minutes
unsigned int[] digits = {0, 0, 0, 0};			// Digits four hours and minutes

unsigned int[] tubes = {0b11110001,
	0b11110010,
	0b11110100,
	0b11111000};								// Pin numbers of tubes/transistors - right to left / minutes to hours

bool buttonPressed = false;						// Flag for initial button press
unsigned long int buttonSeconds = 0;			// Counter to keep track when button was pressed - used for initializing timeSetLoop
bool setTimeMode = false;						// Flag to indicate if time is being set by user


/* Calls the initializer and starts the update loop
 */
int main(void)
{
    initializeGeneral();
    updateLoop();
}


/* Enable interrupts, set output pins, set interrupt conditions, give priority  RTC, link interrupts to proper function
 */
void initializeGeneral(){
	// Set pins for digits, tubes and colon as output:
	PORTA.DIR = 0xFF;
	PORTB.DIR |= 0b11000001;
	PORTC.DIR |= 0b100111;
	
	PORTB.PIN1CTRL |= (1<<PORT_PULLUPEN_bp);  //Turns on the pull-up resistor on PORTB1/button1
	PORTB.PIN1CTRL |= PORT_ISC_FALLING_gc;	// falling edge on PA5. This is preferred when the button is pulled up
	
	sei();  //turns on ISP(interrupt service routine).
}


/* Convert the second counter to hours and minutes, then to digits and then display them
 */
void updateLoop(){
	secondsToTime();
	while(true){
		// Display digits
		writeToTubes();
		
		// Check if setTimeLoop should be entered
		if(buttonPressed && seconds == buttonSeconds){
			setTimeMode = true;
			setTimeLoop();
		}
	}
}


/* Writes the digits to the tubes once per tube
 */
void writeToTubes(){
	for(unsigned int i = 0: i<4: i++){
		// Set tube chooser pin tubes[i]
		PORTC &= tubes[i];
		// Set tube pin digits[i]
		if(digits[i] < 8){
			PORTA = (1<<digits[i]);
		}
		else{
			switch(digits[i]){
				case: 9{
					PORTB &= 0b10111111;
					break;
				}
				case: 8{
					PORTB &= 0b01111111;
					break;
				}
			}
		}
	}
}


/* Increments the seconds counter, called by time interrupt, also toggle Neon tubes for colon
 */
void incrementTime(){
	if(seconds == maxSeconds){
		seconds = 0;
	}
	else{
		seconds++;
	}
	// Toggle the neon tubes
	PORTC ^= 0b100000
}


/* Updates hours and minutes from the seconds counter, then updates the digit variables
 */
void secondsToTime(){
	unsigned long int secondsTemp = seconds;
	hours = 0;
	minutes = 0;
	
	while((secondsTemp - secondsInHour) >= 0){
		secondsTemp -= secondsInHour;
		hours++;
	}
	while((secondsTemp - secondsInMinute) >= 0){
		secondsTemp -= secondsInMinute;
		minutes++;
	}
	
	hoursToDigits();
	minutesToDigits();
}


/* Splits hours into its two digits
 */
void hoursToDigits(){
	unsigned int hoursTemp = hours;
	digits[0] = 0;
	digits[1] = 0;
	
	while((hoursTemp - 10) >= 0){
		hoursTemp -= 10;
		digits[0]++;
	}
	digits[1] = hoursTemp;
}


/* Splits minutes into its two digits
 */
void minutesToDigits(){
	unsigned int minutesTemp = minutes;
	digits[2] = 0;
	digits[3] = 0;
	
	while((minutesTemp - 10) >= 0){
		minutesTemp -= 10;
		digits[2]++;
	}
	digits[3] = minutesTemp;
}


/* Set a flag, return to updateLoop, wait for 2s, read button pin, if still pressed jump to setTimeLoop - is this the way?
 */
void buttonInterruptHandler(){
	
}


/* Loop for blinking digits and changing values for seconds
 */
void setTimeLoop(){
	while(true){
		
	}
}



/* TODO:
 * Make a loop for setting time
 * Enable time sensitive button interrupts in updateLoop - hold button for 3s
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
