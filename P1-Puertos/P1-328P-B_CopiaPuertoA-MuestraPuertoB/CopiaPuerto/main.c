/*
 * P1-B_328P.c
 *
 * Created: 27/02/2022 10:46:52 p. m.
 * Author : cacho-wen
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


int i = 0, val=0;

int main(void)
{
	
	DDRB = 0X00;
	PORTB = 0XFF;
	DDRD = 0XFF;
    /* Replace with your application code */
    while (1) 
    {
		val =PINB;
		_delay_us(500);
		PORTD = val;
		_delay_ms(250);
		
		
    }
}
