/*
Author: Jarvis Bui
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}
void TimerOff(){
	TCCR1B = 0x00;
}
void TimerISR(){
	TimerFlag = 1;
}
ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void TickTL();
void TickBL();
void TickCL();
enum T_States{T_start, led1, led2, led3}T_state;
enum B_States{B_start, on, off}B_state;
enum C_States{C_start, out}C_state;
unsigned char blink = 0x00;
unsigned char threeLeds = 0x00;
unsigned char temp = 0x00;
unsigned short count1 = 0x00;
unsigned short count2 = 0x00;

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	
	T_state = T_start;
	B_state = B_start;
	C_state = C_start;

	count1 = 0;
	count2 = 0;

	TimerSet(1);
	TimerOn();
    /* Insert your solution below */
    while (1) {
	TickTL();
	TickBL();
	TickCL();
	while(!TimerFlag);
	TimerFlag = 0;	
    }
    return 1;
}

void TickTL(){
	switch(T_state){//transitions
		case T_start:
			T_state = led1;
			break;

		case led1:
			if(count1 < 300){
				T_state = led1;
				count1++;
			}
			else{
				T_state = led2;
				count1 = 0;
			}
			break;

		case led2:
			if(count1 < 300){
				T_state = led2;
				count1++;
			}
			else{
				T_state = led3;
				count1 = 0;;
			}
			break;

		case led3:
			if(count1 < 300){
				T_state = led3;
				count1++;
			}
			else{
				T_state = led1;
				count1 = 0;
			}
			break;

		default:
			break;

	}
	switch(T_state){//actions
		case T_start:
			break;

		case led1:
			threeLeds = 0x01;
			break;

		case led2:
			threeLeds = 0x02;
			break;

		case led3:
			threeLeds = 0x04;
			break;

		default:
			break;
	}
}

void TickBL(){
	switch(B_state){//transitions
		case B_start:
			B_state = on;
			break;

		case on:
			if(count2 < 1000){
				B_state = on;
				count2++;
			}
			else{
				B_state = off;
				count2 = 0;
			}
			break;

		case off:
			if(count2 < 1000){
				B_state = off;
				count2++;
			}
			else{
				B_state = on;
				count2 = 0;
			}
			break;

		default:
			break;
	}
	switch(B_state){//actions
		case B_start:
			break;
		case on:
			blink = 0x08;
			break;
		case off:
			blink = 0x00;
			break;
		default:
			break;		
	}
}

void TickCL(){
	switch(C_state){//transitons
		case C_start:
			C_state = out;
			break;

		case out:
			break;

		default:
			break;
	}
	switch(C_state){//actions
		case C_start:
			break;

		case out:
			PORTB = threeLeds | blink;
			break;
	}

}
