/*
 * main.c
 *
 * Created: 2/24/2021 1:14:51 PM
 *  Author: someb
 */ 

#include <xc.h>
#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BIT(x)			(1 << (x))

// wait(): busy waiting for 'ms' millisecond
// Used library: util/delay.h
void wait( int ms ) {
	for (int tms=0; tms<ms; tms++) {
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}

int HighModeEnabled = 1;

int const HighCountTime = 150;
int const LowCountTime = 250;
int currentCountTime;


ISR(TIMER2_COMP_vect){
	if(HighModeEnabled){
		//Entering low mode
		HighModeEnabled = 0;
		currentCountTime = LowCountTime;
		PORTD = 0x00;
		
	}else{
		//Entering High mode
		HighModeEnabled = 1;
		currentCountTime = HighCountTime;
		
		PORTD = 0xFF;
	}
	OCR2 = currentCountTime;
}

void initTimer2(void){
	
	OCR2 = 150;				// Compare value of counter 2
	TIMSK |= BIT(7);		// T2 compare match interrupt enable
	sei();
	TCCR2 = 0b00001101;		//prescale 1024 COMP
}

int main(void)
{
	DDRD = 0xFF;
	initTimer2();
	currentCountTime = HighCountTime;
	
    while(1)
    {
        //TODO:: Please write your application code 
		wait(10);
    }
}