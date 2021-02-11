#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

char data[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00};
int myIndex = -1;
/****************************************************************** 
short:			Busy wait number of millisecs
inputs:			int ms (Number of millisecs to busy wait)
outputs:	
notes:			Busy wait, not very accurate. Make sure (external)
				clock value is set. This is used by _delay_ms inside
				util/delay.h
Version :    	DMK, Initial code
*******************************************************************/
void wait( int ms ) {
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

void turn_on_lights( void ){
	PORTC = data[myIndex];
}

/*****************************************************************
short:			ISR INT0
inputs:
outputs:
notes:			Set PORTD.5
Version :    	DMK, Initial code
*******************************************************************/
ISR( INT1_vect ) {
	myIndex++;
	if (myIndex >= (sizeof(data)/sizeof(data[0]))){
		myIndex = 0;
	}	
	turn_on_lights();	
}

/******************************************************************
short:			ISR INT1
inputs:
outputs:
notes:			Clear PORTD.5
Version :    	DMK, Initial code
*******************************************************************/
ISR( INT2_vect ) {
    myIndex--;
    if (myIndex < 0 ){
	    myIndex = (sizeof(data)/sizeof(data[0])) - 1;
    }
	turn_on_lights();	
}

/******************************************************************
short:			main() loop, entry point of executable
inputs:
outputs:
notes:			Slow background task after init ISR
Version :    	DMK, Initial code
*******************************************************************/
int main( void ) {
	// Init I/O
	DDRC = 0xFF;			//set port D for output

	// Init Interrupt hardware
	EICRA |= 0x28;			// INT1 falling edge
	EIMSK |= 0x06;			// Enable INT1
	
	
	// Enable global interrupt system
	//SREG = 0x80;			// Of direct via SREG of via wrapper
	sei();				

	while (1) {
										
	}

	return 1;
}