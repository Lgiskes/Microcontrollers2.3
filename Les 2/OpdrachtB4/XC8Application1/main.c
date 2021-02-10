/*
 * main.c
 *
 * Created: 2/10/2021 2:04:58 PM
 *  Author: someb
 */ 

#define F_CPU 8e6
#include <xc.h>
#include <util/delay.h>

typedef struct{
	unsigned char lightcode;
	unsigned int delay;
	} SHOW_BIT;
	
	SHOW_BIT pattern[] = {
		{0x01, 100},{0x02, 100},{0x04, 100},{0x08, 100},{0x10, 100},{0x20, 100},
		{0x01, 50},{0x03, 50},{0x07, 50},{0x0F, 50},{0x1F, 50},{0x3F, 50},
		{0x3F, 50},{0x1F, 50},{0x0F, 50},{0x07, 50},{0x03, 50},{0x01, 50},
		{0x01, 250},{0x02, 250},{0x40, 250},{0x20, 250},
		{0x00, 1000},
		{0x0, 0}
	};

void wait(int ms){
	for(int i =0; i< ms; i++){
		_delay_ms(1);
	}
}

void showSegment(int index){
	PORTB = pattern[index].lightcode;
	wait(pattern[index].delay);	
}

int main(void)
{
	
	DDRB = 0xFF;
	

    while(1)
    {
		int index = 0;
		while(pattern[index].delay != 0){
				PORTB = pattern[index].lightcode;
				wait(pattern[index].delay);
				//showSegment(index);
				index++;
				wait(1);
		}

    }
}