#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>

#define BIT(x)	(1 << (x))
#define LCD_E 	6  // RA6 UNI-6
#define LCD_RS	4  // RA4 UNI-6

void lcd_strobe_lcd_e(void);
void lcd_write_data(unsigned char byte);
void lcd_write_cmd(unsigned char byte);
void lcd_clear(void);
void display_text(const char *str);
void set_cursor(int position);
void lcd_init(void);

int distance0;
int distance1;
int measuring = 0;

void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}

// Initialize ADC: 8-bits (left justified), free running
void adcInit( void )
{
	ADMUX = 0b01100001;			// AREF=VCC, result left adjusted, channel 1 at pin PF1
	ADCSRA = 0b11000110;		// ADC-enable, no interrupt, start, free running, division by 64
}

// Main program: ADC at PF1
int main( void )
{
	DDRF = 0x00;				// set PORTF for input (ADC)
	DDRD = 0xFF;				// set PORTD for output
	adcInit();					// initialize ADC
	lcd_init();					// initialize LCD

	while (1){
		PORTD = ADCH;
		char distance_buffer0[20];
		char distance_buffer1[20];
		while(ADCSRA &BIT(6));
		ADMUX ^= BIT(1);
		ADCSRA |= BIT(6);
		measuring ^= 1;
		if (measuring){
			distance1 = ADCH;
			sprintf(distance_buffer0, "Distance: %i", distance1);
			} else {
			distance0 = ADCH;
			sprintf(distance_buffer1, "Distance: %i", distance0);
		}
		lcd_clear();
		wait(5);
		set_cursor(0);
		wait(5);
		display_text(distance_buffer0);
		wait(5);
		set_cursor(16);
		wait(5);
		display_text(distance_buffer1);
		wait(500);
	}
}

void lcd_strobe_lcd_e(void) {
	PORTA |= (1<<LCD_E);	// E high
	_delay_ms(1);			// nodig
	PORTA &= ~(1<<LCD_E);  	// E low
	_delay_ms(1);			// nodig?
}

void lcd_init(void) {
	// PORTC output mode and all low (also E and RS pin)
	//DDRD = 0xFF;
	DDRC = 0xFF;
	DDRA = 0xFF;
	PORTC = 0x00;
	PORTA = 0x00;
	//PORTA = 0xFF;

	// Step 2 (table 12)
	PORTC = 0x20;	// function set
	lcd_strobe_lcd_e();

	// Step 3 (table 12)
	PORTC = 0x20;   // function set
	lcd_strobe_lcd_e();
	PORTC = 0x80;
	lcd_strobe_lcd_e();

	// Step 4 (table 12)
	PORTC = 0x00;   // Display on/off control
	lcd_strobe_lcd_e();
	PORTC = 0xF0;
	lcd_strobe_lcd_e();

	// Step 4 (table 12)
	PORTC = 0x00;   // Entry mode set
	lcd_strobe_lcd_e();
	PORTC = 0x60;
	lcd_strobe_lcd_e();
}

void display_text(const char *str) {
	// Het kan met een while:

	// while(*str) {
	// 	lcd_write_data(*str++);
	// }

	// of met een for:
	for(;*str; str++){
		lcd_write_data(*str);
	}
}

void lcd_write_data(unsigned char byte) {
	// First nibble.
	PORTC = byte;
	PORTA |= (1<<LCD_RS);
	lcd_strobe_lcd_e();

	// Second nibble
	PORTC = (byte<<4);
	PORTA |= (1<<LCD_RS);
	lcd_strobe_lcd_e();
}

void lcd_write_command(unsigned char byte) {
	// First nibble.
	PORTC = byte;
	PORTA &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e();

	// Second nibble
	PORTC = (byte<<4);
	PORTA &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e();
}

void lcd_clear() {
	lcd_write_command (0x01);						//Leeg display
	_delay_ms(2);
	lcd_write_command (0x80);						//Cursor terug naar start
}

void set_cursor(int position){
	if(position >= 0 && position <= 15){
		position += 0x80;
		lcd_write_command(position);
	}
	else{
		if(position >= 16 && position <= 31){
			position+=0x80+48;
			lcd_write_command(position);
		}
	}
}