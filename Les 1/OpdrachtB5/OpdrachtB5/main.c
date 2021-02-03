/*
 * main.c
 *
 * Created: 2/3/2021 3:04:49 PM
 *  Author: jkbro
 */ 

#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>

typedef struct {
	unsigned char data;
	unsigned int delay ;
} PATTERN_STRUCT;

PATTERN_STRUCT pattern[] = {
	{0x00, 100}, {0x01, 100}, {0x02, 100}, {0x04, 100}, {0x10, 100}, {0x20, 100}, {0x40, 100}, {0x80, 100}, //led 0 - 7
	{0x80, 50}, {0x40, 50}, {0x20, 50}, {0x10, 50}, {0x4, 50}, {0x02, 50}, {0x01, 50}, {0x00, 50}, //led 7 - 0
	{0x18, 100}, {0x24, 100}, 	{0x42, 100}, {0x81, 100}, //inner leds to outer leds
	{0x81, 50}, {0x42, 50}, 	{0x24, 50}, {0x18, 50}, //outer leds to inner leds
	{0x18, 100}, {0x24, 100}, 	{0x42, 100}, {0x81, 100}, //inner leds to outer leds
	{0x81, 50}, {0x42, 50}, 	{0x24, 50}, {0x18, 50},	//outer leds to inner leds
	{0x00, 0x00} //all leds off
};

/*Busy wait number of millisecs*/
void wait( int ms ) {
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

int main(void)
{
	DDRD = 0b11111111;	//set port D for output
    while(1)
    {
        // Set index to begin of pattern array
        int index = 0;
        // as long as delay has meaningful content
        while( pattern[index].delay != 0 ) {
	        // Write data to PORTD
	        PORTD = pattern[index].data;
	        // wait
	        wait(pattern[index].delay);
	        // increment for next round
	        index++;
        }
    }
	return 1;
}