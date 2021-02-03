/*
 * main.c
 *
 * Created: 2/3/2021 1:58:10 PM
 *  Author: jkbro
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#define BIT(x) (1 << (x))

void wait (int ms)
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}


int main(void)
{
	DDRD = 0b10000000; //PORT D is output
	DDRC = 0b00000000; // PORTC is input
	//PORTD &= ~BIT(7); // PD7 = 0
	
	while (1)
	{
		if (PINC & 0x01)
		{
			PORTD = 0x80;
			wait(500);
			PORTD = 0x00;
			wait(500);
		}
	}
	return 1;
}