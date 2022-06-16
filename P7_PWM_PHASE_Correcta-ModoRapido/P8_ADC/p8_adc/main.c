/*
 * p8_adc.c
 *
 * Created: 10/06/2022 05:12:08 p. m.
 * Author : cacho-wen
 */ 


#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include <util/delay.h>


char str[16];
int main(void)
{
	
	DDRD  = 0x00;						// ENTRADAS - INT0 Y 1
	PORTD = 0xFF;						// Pull up
	DDRB  = 0xFF;						// LCD
	LCD_INICIALIZA();
	
	_delay_ms(20);
	
	ENVIA_CADENA("Valor medido: ");
	_delay_ms(20);
	cli();
	
	DDRB |= (1<<DDB5);
	ADC_init();
	
	sei();
	
	while (1)
	{
		float adcV = ADC_GetData(0)*5.0f/1024.0f;
		
		
		ENVIA_CADENA(str);
		
		
		if (adcV > 3.0f)
		{
			PORTB |= (1<<PORTB5);
		}
		else
		{
			PORTB &=~ (1<<PORTB5);
		}
	}
    /* Replace with your application code */
    
}


void ADC_init()
{
	// Output adjust = right //
	ADMUX &=~ (1<<ADLAR);

	// Voltage Reference = AVCC //
	ADMUX |=  (1<<REFS0);
	ADMUX &=~ (1<<REFS1);

	// Frequency divisor = 128 -> 16000/128 = 125 KHz
	ADCSRA |= (1<<ADPS0);
	ADCSRA |= (1<<ADPS1);
	ADCSRA |= (1<<ADPS2);
}

int ADC_GetData(int canal)
{
	// Selección del canal de lADC //
	ADMUX &=~  0x0F;
	ADMUX |=  canal;
	
	// Encendemos en ADC
	ADCSRA |= (1<<ADEN);
	_delay_us(10);	// Esperamos a que caliente

	// Mandamos el muestreo
	ADCSRA |= (1<<ADSC);

	// Esperamos a que muestree, leyendo el flag
	while( !(ADCSRA & (1<<ADIF)) );
	ADCSRA |= (1<<ADIF);	// Reiniciamos el flag

	// Apagamos el ADC
	ADCSRA &=~ (1<<ADEN);

	return ADC;
}
