/*
 * main.c
 *
 * Created: 2/3/2021 7:04:20 PM
 *  Author: jkbro
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

/*function prototypes*/
void s1(void);
void s2(void);
void s3(void);
void end(void);
void start(void);

/*Define fsm (events and states*/
typedef enum {PortD5, PortD6, PortD7} EVENTS;
typedef enum {START, STATE1, STATE2, STATE3, END} STATES;
	
/*Define fsm transition*/
typedef struct {
	void (*body)(void);
	STATES nextState;
} STATE_TRANSITION_STRUCT;

STATE_TRANSITION_STRUCT fsm[5][3] = {
	{{s2, STATE2},	{s1, STATE1},	{start, START}},
	{{s2, STATE2},	{s1, STATE1},	{start, START}},
	{{s3, STATE3},	{s1, STATE1},	{start, START}},
	{{end, END},	{end, END},		{start, START}},
	{{end, END},	{end, END},		{start, START}}
};

STATES currentState = START;

void handleEvent(EVENTS event){
	fsm[currentState][event].body();
	currentState = fsm[currentState][event].nextState;
}

void s1(){
	printf("s1\n");
}

void s2(){
	printf("s2\n");
}

void s3(){
	printf("s3\n");
}

void start(){
	printf("start\n");
}

void end(){
	printf("end\n");
}

int main(void)
{
	handleEvent(PortD5);
	handleEvent(PortD6);
	handleEvent(PortD7);
	handleEvent(PortD6);
	handleEvent(PortD5);
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
	return 1;
}