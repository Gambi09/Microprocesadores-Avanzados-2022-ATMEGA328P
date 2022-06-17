/*
 * P9-ComparadorAnalogico.c
 *
 * Created: 14/06/2022 05:21:46 p. m.
 * Author : cacho-wen
 */ 
#define F_CPU 16000000UL//Frecuencia de reloj del microcontrolador

#include <avr/io.h>
#include <util/delay.h>
#include "lcdA.h"
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>

char str[4];

int triggered=0;
unsigned int adcIn = 0;
float volts = 0;

void printADC(unsigned int inC);
void printVolts(float inC);
ISR(ANALOG_COMP_vect)
{
	triggered=~triggered;
	ACSR|=(1<<ACI);

}
ISR(ADC_vect){
	adcIn =  ADCL | ADCH<<8;
	ADCSRA |= (1<<ADSC);

}

int main(void)
{
	cli();
	
	DDRC=0x00; //Entradas analógicas
	DDRB = 0xFF;	// Salidas para LCD
	DDRD=0x00;
	ACSR|=(1<<ACIE)|(1<<ACI)|(1<<ACIS1);
	//ADC-01
	ADCSRB |= (1<<ADTS2) | (0<<ADTS1) | (1<<ADTS0);//Desbordamiento T0
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);//Factor 1:128
	//ADC-02
	//ADCSRB |= (0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);//Corrida libre
	//ADCSRA |= (0<<ADPS2) | (0<<ADPS1) | (1<<ADPS0);//Factor 1:1
	
	ADCSRA |= (1<<ADEN) | (1<<ADIE) | (0<<ADATE);
	ADMUX |= (0<<REFS1) | (1<<REFS0) | (0<<ADLAR); //Ref, 5v
	ADCSRB=0;
	
	sei();
	
	_delay_ms(100);
	LCD_INICIALIZA();
	_delay_ms(100);
	LIMPIA_LCD();
	REGRESO_CASA();
	ENVIA_CADENA("Vol:   ");
	POS_LINEA2(0);
	ENVIA_CADENA("ADC:    ");
	
	
	ADCSRA |= (1<<ADSC);
	
	while(1){
		if (ACIE==1)
		{
			triggered=0;
		}
		if (triggered==0)
		{
			printADC(adcIn);
			if (adcIn > 0)
			{
				volts = ((float)adcIn*5)/1024;
				} else{
				volts = 0;
			}
			printVolts(volts);
		}
		else
		{
			printADC(0);
			volts = 0;
			printVolts(volts);
			
		}
	}
}

void printADC(unsigned int inC){
	REGRESO_CASA();
	POS_LINEA2(10);
	ENVIA_CADENA(dtostrf(inC,1,2,str));

};

void printVolts(float inC){
	POS_LINEA1(10);
	ENVIA_CADENA(dtostrf(inC,1,2,str));
	ENVIA_CADENA(" v");
};


