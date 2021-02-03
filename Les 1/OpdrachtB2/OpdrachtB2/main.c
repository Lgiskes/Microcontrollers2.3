/*
 * main.c
 *
 * Created: 2/3/2021 1:41:42 PM
 *  Author: someb
 */ 

#include <xc.h>
#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>

void wait(int msec){
	for(int i =0; i<msec; i++ ){
	_delay_ms(1);
	}
}

int main(void)
{
	DDRD = 0b01100000;
	PORTD = 0x00; //turn all off
	PORTD = 0b01000000; //turn 7 on
	
    while(1)
    {
		wait(500);
		PORTD = PORTD ^ (1<<6); //switch 7
		PORTD = PORTD ^ (1<<5);	//switch 6
		
        //TODO:: Please write your application code 
		
    }
}