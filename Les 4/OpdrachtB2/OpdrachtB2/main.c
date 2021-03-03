/*
 * Project name:
     Demo4_1 : Free running AD conversion on channel 1, PORTF.1
 * Author: Avans-TI, JW
 * Revision History: 
     20101229: - initial release;
 * Description:
     This program gives an interrupt on each ms
 * Test configuration:
     MCU:             ATmega128
     Dev.Board:       BIGAVR6
     Oscillator:      External Clock 08.0000 MHz
     Ext. Modules:    -
     SW:              AVR-GCC
 * NOTES:
     - Turn ON the PORT LEDs at SW12.1 - SW12.8
*/

#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BIT(x)	(1 << (x))

// wait(): busy waiting for 'ms' millisecond
// Used library: util/delay.h
void wait( int ms ) {
	for (int tms=0; tms<ms; tms++) {
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}

ISR (INT0_vect){
	ADCSRA |= BIT(6);				// Start ADC
}


// Initialize ADC: 8-bits (left justified), no free running
void adcInit( void ) {
	ADMUX = 0b01100011;			// AREF=VCC, result left adjusted, positive channel3 at pin PF3, negative channel2 at pinPF2, Gain 10x
	ADCSRA = 0b11000110;		// ADC-enable, no interrupt, start, no free running, division by 64
}


// Main program: ADC at PF1
int main( void ) {
	DDRF = 0x00;					// set PORTF for input (ADC)
	DDRA = 0xFF;
	
	EICRA |= 0b00000011;
	EIMSK |= 0x01;
	sei();
	
	adcInit();						// initialize ADC

	while (1) {
		while ( ADCSRA & BIT(6) ) ;		// Wait for completion
		PORTA = ADCH;					// Show MSB (bit 9:2) of ADC
		wait(500);						// every 500 ms (busy waiting)
	}
}