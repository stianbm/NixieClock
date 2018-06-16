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
static unsigned int button = 0b00100000;

#define Button1_check (!(PORTB.IN & button))  //button1 check to make it short and more understandable in the code.

//Defining my own boolean commands
#define TRUE 1
#define FALSE 0
//Creating flags for button1 to keep it from being activated multiple times if it is being held down
uint8_t Button1_flag = FALSE;


void toggleLED(){
	PORTB.OUT ^= LED;
}


void initializeLED(){
	PORTB.DIR |= LED;	// Set PB4/LED0 as output
	PORTB.OUT |= LED;	// Set 1 as default value (off)
}


void initializeButton(){
	PORTB.PIN5CTRL |= (1<<3);  //Turns on the pull-up resistor on PORTB5
	PORTB.PIN5CTRL |= PORT_ISC_FALLING_gc;	// falling edge on PA5. This is preferred when the button is pulled up
}


//this button reads the button value and sets the flag low if its not pressed
void Button_check(void)
{
	if (!Button1_check)
	{
		Button1_flag = FALSE; //Sets the flag low
	}
}


int main(void)
{
	initializeLED();
	initializeButton();
	sei();  //turns on ISP(interrupt service routine).
	while (1)
	{
		Button_check(); //Checks button status
	}
}


ISR(PORTB_PORT_vect)
{
	//checks the status of button 1. It should be high, but this ISR gets activated by any PORTA interrupt
	if (Button1_check)
	{
		if (Button1_flag ==  FALSE)
		{
			Button1_flag = TRUE; //Sets the button flag high
			toggleLED();
			_delay_ms(500); //Waits for 500ms

		}
	}
}