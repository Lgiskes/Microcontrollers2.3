#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define BIT(x)	(1 << (x))
#define LCD_E 	6  // RA6 UNI-6
#define LCD_RS	4  // RA4 UNI-6
#define MAX_VIBRATO 10
#define VIBRATO_STEP_AMOUNT 0.01

void lcd_strobe_lcd_e(void);
void lcd_write_data(unsigned char byte);
void lcd_write_cmd(unsigned char byte);
void lcd_clear(void);
void display_text(const char *str);
void set_cursor(int position);
void lcd_init(void);
void calculateFrequency(void);

int hertz_percentage;
int vibrato_percentage;
int measuring = 0;
int hertz_value;
int average_percentages_hertz[10] = {0,0,0,0,0,0,0,0,0,0};
int average_percentages_vibrato[10] = {0,0,0,0,0,0,0,0,0,0};
int bufferList[10];
double upperVibratoPercentage = 100;
double lowerVibratoPercentage = 100;
double currentPercentage = 100;
int goingLowToHigh = 1;
int currentHertz = 0;

//sets the buzzer high and low
ISR (TIMER2_COMP_vect){
	PORTG ^= BIT(0);
}


void timersInit( void ){
	OCR2 = 250; //standard time
	TIMSK |= BIT(7);
	sei();
	TCCR2 = 0b00001011; //interrupt enable, prescaler 64x
}


void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}

/*
Adds an item to an existing list
int list_to_add[]:	The list where the item is added to
int value:			The value to be added to the list
*/
void addToList(int list_to_add[], int value){
		int elementAmount = 10;
		if(elementAmount < 10){
			list_to_add[elementAmount] = value;
			return;
		} else {
			for (int i = 0; i < 10; i++){
				bufferList[i] = list_to_add[i];
			}
			for (int i = 0; i < 9; i++){
				list_to_add[i] = bufferList[i + 1];
			}
			list_to_add[9] = value;
		}
		
}
/*
Returns the avarage value of the items in an existing list
int list_to_avarage[]:	The list which avarage is to be calculated
int return:				The avarage value of the list
*/

int average_of_list(int list_to_average[]){
	int size = 10;
	int sum = 0;
	for (int i = 0; i < size; i++){
		sum += list_to_average[i];
	}
	return sum / size;
}

// Initialize ADC: 8-bits (left justified), single entry mode
void adcInit( void )
{
	ADMUX = 0b01100001;			// AREF=VCC, result left adjusted, channel 1 at pin PF1
	ADCSRA = 0b11000110;		// ADC-enable, no interrupt, start, single entry mode, division by 64
}

// interpolates the value between 0 and 100
int returnDistance(int distance){
	int value = 0;
	if (distance < 0){
		value = 0;
	} else if(distance > 100){
		value = 100;
	} else{
		value = distance;
	}
	return value;
}

//Returns the frequency 
int returnFrequency(int value){
	return (int)(pow(2, (double)(value - 49) / 12.0) * 440);
}

int percentageToKey(int percentage){
	return 40 + (percentage/10); //range from key 40 to 50
}

void setVibratoRange(int vibrato_percentage){
	int vibrato_range = (((100-vibrato_percentage)*MAX_VIBRATO)/100);
	upperVibratoPercentage = 100 + vibrato_range;
	lowerVibratoPercentage = 100 - vibrato_range;
}

void calculateFrequency(){
	
	if(currentPercentage == upperVibratoPercentage && currentPercentage == lowerVibratoPercentage){
		currentHertz = hertz_value;
	}
	if(goingLowToHigh){
		
		if(currentPercentage >= upperVibratoPercentage){
			goingLowToHigh = 0;
		}
		else{
			currentPercentage += VIBRATO_STEP_AMOUNT;
		}
	}
	else{
		
		if(currentPercentage <= lowerVibratoPercentage){
			
			goingLowToHigh = 1;
		}
		else{
			currentPercentage -= VIBRATO_STEP_AMOUNT;
		}
	}
	
	currentHertz = hertz_value*(currentPercentage/100.0);
}

// Main program: ADC at PF1
int main( void )
{
	DDRF = 0x00;				// set PORTF for input (ADC)
	DDRD = 0xFF;				// set PORTD for output
	DDRG = 0xFF;				// set PORTG for output
	adcInit();					// initialize ADC
	//lcd_init();					// initialize LCD
	timersInit();

	while (1){
		PORTD = ADCH;
		char hertz_buffer[20];
		char vibrato_buffer1[20];
		while(ADCSRA &BIT(6));
		ADMUX ^= BIT(1);
		ADCSRA |= BIT(6);
		measuring ^= 1;
		if (measuring){
			hertz_percentage = returnDistance(140-ADCH);
			addToList(average_percentages_hertz, hertz_percentage);
			hertz_value = returnFrequency(percentageToKey( average_of_list(average_percentages_hertz)));

			sprintf(hertz_buffer, "Hertz: %i", currentHertz);
			} else {
			vibrato_percentage = returnDistance(140-ADCH);
			addToList(average_percentages_vibrato, vibrato_percentage);
			setVibratoRange(average_of_list( average_percentages_vibrato));
			sprintf(vibrato_buffer1, "Vibrato: %i", average_of_list(average_percentages_vibrato));
			calculateFrequency();
			OCR2 = (250000)/(2*currentHertz);
		}
		
		
		/*
		lcd_clear();
		wait(5);
		set_cursor(0);
		wait(5);
		display_text(vibrato_buffer1);
		wait(5);
		set_cursor(16);
		wait(5);
		display_text(hertz_buffer);
		wait(250);
		*/
		
		
		
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