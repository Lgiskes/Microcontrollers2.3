/*
 * main.c
 *
 * Created: 2/3/2021 3:22:01 PM
 *  Author: someb
 */ 

#include <xc.h>
#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>
#define BIT(x) (1<<(x))

void wait( int ms )
/* 
short:			Busy wait number of millisecs
inputs:			int ms (Number of millisecs to busy wait)
outputs:	
notes:			Busy wait, not very accurate. Make sure (external)
				clock value is set. This is used by _delay_ms inside
				util/delay.h
*******************************************************************/
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}
int main(void)
{
	int FALSE = 0;
	int TRUE = 1;
	
	int usingQuickFrequency = 0; //the state if the quick frequency should be used
	int ispressing = 1;
	int quickFrequency = 25;
	int slowFrequency = 1000;
	int currentFrequency = slowFrequency;
	DDRD = 0x80;
	DDRC = 0x00;
    while(1)
    {
		if(PINC & 0x01){
			if(ispressing == FALSE){ //if the button has been pressed, but has not been held down
				usingQuickFrequency ^= 1; //toggle quick frequency state
				if(usingQuickFrequency == TRUE){ //sets the currentFrequency to the correct speed
					currentFrequency = quickFrequency;
				}
				else{
					currentFrequency = slowFrequency;
				}
			}
			ispressing = TRUE ;	
		}
		else{
			ispressing = FALSE;
		}
		
		PORTD ^= BIT(7); //toggle the led
		wait(currentFrequency);

		
        //TODO:: Please write your application code 
    }
}