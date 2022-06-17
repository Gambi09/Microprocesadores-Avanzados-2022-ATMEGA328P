/*
 * P10-USART.c
 *
 * Created: 16/06/2022 10:08:32 a. m.
 * Author : cacho-wen
 */ 

# define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>
#include "LCDA.h"

#include <stdlib.h>


char count[17];
int data,ban,j=0;


int main(void)
{
	INIT_USART();
	LCD_INICIALIZA(); 
	_delay_ms(10);
	LIMPIA_LCD(); 
	_delay_ms(10);
	POS_LINEA1(0);
	ENVIA_CADENA("Dato de la compu: ");
	POS_LINEA2(3);
	
	
	while (1)
	{
		data=RECEPT_USART();
		_delay_ms(10);
		
			ENVIA_DATO(data);
		
		_delay_ms(100);
	}
}

void SEND_USART( unsigned char data)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

int RECEPT_USART(void)
{
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

void INIT_USART(void){
	
	DDRB = 0xFF;
	UCSR0A = (0<<RXC0)|(0<<TXC0)|(0<<UDRE0)|(0<<FE0)|(0<<DOR0);
	UCSR0A|= (0<<U2X0)|(0<<MPCM0); //UCSR0A= 0x00;
	UCSR0B = (0<<RXCIE0)|(0<<TXCIE0)|(0<<UDRIE0)|(1<<RXEN0)|(1<<TXEN0);
	UCSR0B|= (0<<UCSZ02)|(0<<RXB80)|(0<<TXB80);//UCSR0B = 0b00011000;
	UCSR0C = (0<<UMSEL01)|(0<<UMSEL00)|(0<<UPM01)|(0<<UPM00)|(0<<USBS0);
	UCSR0C|= (1<<UCSZ01) | (1<<UCSZ00) | (0<<UCPOL0);//0b00000110;
	UBRR0H = 0x00;
	UBRR0L = 0x67;
}