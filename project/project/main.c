/*
 * project.c
 *
 * Created: 12/14/2020 1:09:56 PM
 * Author : Jonathan Ziesmer
 */ 

#define F_CPU 1000000UL

#define MIN_RANGE 0xD2  // 8 cm or ~13 cm
#define MAX_RANGE 0xF0  // ~10.5 cm
#define OUT_RANGE 0xA6  // 21.5 cm

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>


uint8_t GetDist(void)
{
	// PB6 - Vin to sensor
	// PB5 - Out of sensor
	
	DDRB |= (1<<DDB6);  // set PB6 low
	_delay_ms(20);
	while(!(PINB & 0x20)){;}  // wait for PB5 to go high
	DDRB &= ~(1<<DDB6);  // set PB6 high
	_delay_us(100);  // wait .1ms
	
	uint8_t counter = 0;
	uint8_t data = 0x00;
	while(counter < 8)
	{
		DDRB |= (1<<DDB6);  // set PB6 low
		_delay_us(100);  // wait .1ms
		uint8_t temp = (PINB & 0x20) >> 5;  // mask with PB5
		data = (data << 1) | temp;
		DDRB &= ~(1<<DDB6);  // set PB6 high
		_delay_us(100);  // wait .1ms
		counter ++;
	}
	
	_delay_ms(2);  // wait 1.5 or more to put sensor in sleep mode
	
	return data;
}


int main(void)
{
	// PC7 - "on/off" switch
	// PB6 - Sensor Vin
	// PB5 - Sensor output
	// PB7 - servo
	
	DDRC = (0<<DDC7);  // set PC7 to input	
	PORTC = (1<<PC7);  // set PC7 to pullup
	
	DDRB = (1<<DDB6) | (1<<DDB7);  // set PB6 & PB7 to output, set PB5 to input
	PORTB = (1<<PB7) | (0<<PB6);  // set PB7 high, PB6 low
	
	
	// timer PWM init
	TCCR0A = (1<<COM0A0) | (1<<COM0A1) | (1<<WGM01) | (1<<WGM00);  // clear OC0A at match, set timer to fast PWM, top defaults to 0xFF
	
	int moved = 1;
	
    while (1) 
    {
		while(!(PINC & 0x80)) {;}  // wait while PC7 not high
		
		uint8_t dist = GetDist();
		if(dist > MIN_RANGE && dist < MAX_RANGE)  // if user within range
		{
			OCR0A = 37;  // set servo position
			TCCR0B = (1<<CS01) | (1<<CS00);  // start timer PWM
			moved = 0;			
		}
		else if(dist < MIN_RANGE && dist > OUT_RANGE)  // if user moving in/out of range
		{
			OCR0A = 20;  // set servo position
			TCCR0B = (1<<CS01) | (1<<CS00);  // start timer PWM
			moved = 0;			
		}
		else{  // if user out of range
			TCCR0B = 0;  // stop timer
		}
		
		if(!moved)
		{
			_delay_ms(450);  // wait for servo to finish motion
			moved = 1;
		}
    }
}


