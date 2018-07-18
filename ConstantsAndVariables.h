/*
 * ConstantsAndVariables.h
 *
 * Created: 17/07/2018 19:32:01
 *  Author: stian.malmanger
 */ 


#ifndef CONSTANTSANDVARIABLES_H_
#define CONSTANTSANDVARIABLES_H_

// Global constants
static uint8_t maxSeconds = 59; 				// Max value for seconds counter
static uint8_t maxMinutes = 59; 				// Max value for minutes counter
static uint8_t maxHours = 23; 					// Max value for hours counter

static uint8_t clearDigitPortA = 0b00000001;	// Mask to be and'ed to clear digits on port A
static uint8_t clearDigitPortB = 0b00111110;	// Mask to be and'ed to clear digits on port B
static uint8_t clearTubeSelect = 0b11110000;	// Mask to be and'ed to clear tube select on port C

// Global variables
static uint8_t hours = 0;						// Counter for hours
static uint8_t minutes = 0;						// Counter for minutes
static uint8_t seconds = 0;						// Counter for seconds

static uint8_t digits[4] = {0, 0, 0, 0};		// Digits four hours and minutes

static uint8_t hoursTemp = 0;					// Temp used in hoursToDigits()
static uint8_t minutesTemp = 0;					// Temp used in minutesToDigits()

// Pin numbers of tubes/transistors on port C - right to left / minutes to hours
static uint8_t tubes[4] = {0b00000001,
	0b00000010,
	0b00000100,
	0b00001000};

// 8b variable where digit 0 and 1 are mode flags
static uint8_t modeFlags = 0x00;

// Mode flag masks
static uint8_t hourModeFlag = 0x01;
static uint8_t minuteModeFlag = 0x02;

// Start and stop for tubes to be selected in for loop
uint8_t tubesStart = 0;		// Set to 2 to not show minutes
uint8_t tubesStop = 4;		// Set to 2 to not show hours

// Pin masks:
// Port B
uint8_t zero = 0b00000000;
uint8_t modeButton = 0b00000001;
uint8_t nine = 0b01000000;
uint8_t eight = 0b10000000;
// Port C
uint8_t valueButton = 0b00010000;
uint8_t colon = 0b00100000;

#endif /* CONSTANTSANDVARIABLES_H_ */