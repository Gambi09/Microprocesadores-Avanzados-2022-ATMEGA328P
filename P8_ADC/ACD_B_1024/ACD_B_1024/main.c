/*
 * P8-ACD-B_1024.c
 *
 * Created: 12/06/2022 03:32:41 p. m.
 * Author : cacho-wen
 */
 

# define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>
#include "LCDA.h"
#include "ADC.h"

int array2int(int arr[]);
int char2int(char c);
char int2char(int i);
void printstr(int n[],size_t sz);
int * int2array(int number);
int numsz(int number);
char str[4];

int triggered=0;
unsigned int adcIn = 0;
float volts = 0;
int main(void)
{
	double intpart;
	double x;
	double fracp;
	float adcV;
	float adcV2;
	float iadc;
	float iadc2;
	DDRB = 0xFF;
	
	DDRD = 0xF0;
	
	DDRC |= (1<<DDC2);
	
	cli();
	ADC_init();
	sei();
	LCD_INICIALIZA();
	LIMPIA_LCD();
	CAR_ESP0();
	_delay_ms(100);
	cursor_blink_on(0);
	POS_LINEA1(0);
	
	while (1)
	{
		iadc=ADC_GetData(0);
		adcV = iadc*5.0/1024.0;
		_delay_ms(10);
		POS_LINEA1(0);
		ENVIA_CADENA("Voltaje:       ");
		POS_LINEA2(5);
		ENVIA_CADENA(dtostrf(iadc,1,2,str));

		_delay_ms(1000);
		
	}
}


void printstr(int n[],size_t sz)
{
	int i=0;
	for (i=0;i<sz;i++)
	{
		ENVIA_DATO(int2char(n[i]));
	}
}

int array2int(int a[3])
{
	int n=3;
	int i,k=0;
	for (i=0;i<n+1;i++)
	{
		k=10*k+a[i];
	}
	return k;
}
int char2int(char c)
{
	int num = 0;
	num = c - '0';
	return num;
}
char int2char(int i)
{
	char c = i + '0';
	return c;
}
int * int2array(int number)
{
	int n = log10(number)+1;
	int i;
	int *numberArray = calloc(n, sizeof(int));
	for ( i = 0; i < n; ++i, number /= 10 )
	{
		numberArray[n-i-1] = number % 10;
	}
	return numberArray;
}

int numsz(int number)
{
	int n = log10(number)+1;
	int *numberArray = calloc(n, sizeof(int));
	return n;
}


