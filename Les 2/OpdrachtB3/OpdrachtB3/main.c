/*
 * main.c
 *
 * Created: 2/10/2021 2:04:58 PM
 *  Author: someb
 */ 

#define F_CPU 8e6
#include <xc.h>
#include <util/delay.h>

const unsigned int Numbers[16] = {
	0b00111111, //0
	0b00000110, //1
	0b01011011, //2
	0b01001111, //3
	0b01100110, //4
	0b01101101, //5
	0b01111101, //6
	0b00000111, //7
	0b01111111, //8
	0b01101111, //9
	0b01110111, //A
	0b01111100, //b
	0b00111001, //C
	0b01011110, //d
	0b01111011, //E
	0b01110001  //F
};

void showSegment(int number){
	if(number >= 0 && number <= 15){
		PORTB = Numbers[number];
	}
	else{
		PORTB = 0b01111001;
	}
	
}

int main(void)
{
	int index = 0;
	DDRB = 0xFF;
	DDRC = 0x00;
	
	showSegment(index);
    while(1)
    {
		
		if(PINC == 0x03){
			index = 0;
			showSegment(index);
		}
		else{
			if(PINC & 0x01){
				index --;
				showSegment(index);
			}
			if(PINC & 0x02){
				index ++;
				showSegment(index);
			}
		}
		_delay_ms(750);
        //TODO:: Please write your application code 
    }
}