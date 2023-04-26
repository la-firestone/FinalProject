/*********************************************************************
LCD Driver Functions
by: Elliott Gurrola

	LCD Pinouts

	Pin 1	Ground
	Pin 2	VCC (+3.3 to +5V)
	Pin 3	Contrast adjustment  -> 5kOhm
	Pin 4	Register Select (RS). 0: Command, 1: Data -> P4.5
	Pin 5	Read/Write (R/W). 0: Write, 1: Read	  -> GND
	Pin 6	Clock (Enable). Falling edge triggered -> P4.4
	Pin 7	---
	Pin 8	---
	Pin 9	---
	Pin 10	---
	Pin 11	Bit 4 -> P4.0
	Pin 12	Bit 5 -> P4.1
	Pin 13	Bit 6 -> P4.2
	Pin 14	Bit 7 -> P4.3
	Pin 15	Backlight Anode (+) 100Ohm -> VCC
	Pin 16	Backlight Cathode (-)

	Top level functions available
		- lcd_init();			// Initialize LCD
		- write_byte(char c);		// Send a single character (1 byte)
	 	- write_string(char s[]);	// Print a character string
		- write_int(int num);		// Write an integer
		- gotoXy(x, y);			// Move cursor to (x, y) location (Lines and columns start at 0)
		- clear_lcd();			// Clear LCD and move cursor to (0, 0)

	Not important to the user:
		- send_cmd(char cmd);		// Send a command to the LCD
		- write_nibble(char c);		// Send a nibble to the LCD
		- void trigger_EN();

	*** DELAY FUNCTION ASSUMES DCO frequency is 3 MHz CLOCK ***


Modified on: Apr 15, 2013
by: Luiz (Luis Carlos Ba�uelos-Chacon)

Modified on: May 24, 2013
by: Elias N Jaquez

Modified on:  Feb 16, 2019
Port to MSP432
By: R. Romero 
*********************************************************************/

#ifndef LCDLIB_432_H_
#define LCDLIB_432_H_

#include <msp.h>
#include <string.h>
#include <stdio.h>

// Delay Functions
#define delay_ms(x)		__delay_cycles((long) (x) * 3000)
#define delay_us(x)		__delay_cycles((long) (x) * 3)

// Pins
#define EN	BIT4
#define RS	BIT5
#define DATA	0x0F

// Commands
#define CLEAR	0x01

// Functions
void lcdInit();					// Initialize LCD
void lcdTriggerEN();				// Trigger Enable
void lcdWriteData(unsigned char data);		// Send Data (Characters)
void lcdWriteCmd(unsigned char cmd);		// Send Commands
void lcdClear();				// Clear LCD
void lcdSetText(char * text, int x, int y);	// Write string
void lcdSetChar(char text, int x, int y); // Write char
void lcdSetInt(int val, int x, int y);		// Write integer

#endif /* LCDLIB_432_H_ */
