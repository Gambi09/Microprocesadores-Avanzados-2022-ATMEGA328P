/*
 * P6_FrecuenciasTimer0.c
 *
 * Created: 23/04/2022 11:04:31 p. m.
 * Author : cacho-wen
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>


# define F_CPU 16000000UL
# include <util/delay.h>

#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include "lcd.h"

int a, D0, D1;
char str[16];

void initSys(){
	
	DDRD = 0XF0;
	PORTD = 0X0F;
	//External T0 pin Clock on falling edge 1024
	TCCR0B = (0<<WGM02)|(1<<CS02)|(0<<CS01)|(1<<CS00);
	// COM0AX
	// WGM00 PWM ACTIVATE
	TCCR0A = (0<<COM0A1)|(1<<COM0A0)|(0<<WGM00)|(1<<WGM01);
	
	TIMSK0 = (0<<OCIE0B)|(1<<OCIE0A)|(0<<TOIE0);
	OCR0A = 200;
	TCNT0 = 0;
}

void changeFrec(int dato){
	
	switch (dato){
		case (0):
		OCR0A = 155;
		POS_LINEA1(13);
		ENVIA_DATO('1');
		POS_LINEA2(2);
		ENVIA_CADENA(" 50   Hz ");
		
		//ENVIA_CADENA('1');
		break;
		case (1):
		OCR0A = 77;
		POS_LINEA1(13);
		ENVIA_DATO('2');
		POS_LINEA2(2);
		ENVIA_CADENA(" 100  Hz");
		
		
		break;
		case (2):
		OCR0A = 15;
		POS_LINEA1(13);
		ENVIA_DATO('3');
		POS_LINEA2(2);
		ENVIA_CADENA(" 500  Hz");
		
		break;
		case (3):
		OCR0A = 7;
		POS_LINEA1(13);
		ENVIA_DATO('4');
		POS_LINEA2(2);
		ENVIA_CADENA(" 1   kHz ");
		
		break;
		case (4):
		OCR0A = 3;
		POS_LINEA1(13);
		ENVIA_DATO('5');
		POS_LINEA2(2);
		ENVIA_CADENA(" 2   kHz ");
		
		break;
	}
	
}

int main(void)
{
	
	int level=0;
	a = level;
	itoa(a, str, 10);
	LCD_INICIALIZA();
	LIMPIA_LCD();
	POS_LINEA1(2);
	ENVIA_CADENA("Frecuencia ");
	POS_LINEA2(2);
	ENVIA_CADENA(str);
	initSys();
	
	
	while (1)
	{
		D0 = PIND & 0b11111111;
		D1 = PIND & 0b11111111;
		
		if(D0 == 0X0E ){
			//low
			if(level ==0){
				changeFrec (0);
			}
			else{
				level--;
				POS_LINEA2(2);
				//ENVIA_CADENA("baja");
				changeFrec(level);
			}
		}
		
		else if(D1 == 0X0D){
			if (level ==5){
				changeFrec(5);
			}
			else{
				level ++;
				POS_LINEA2(2);
			//	ENVIA_CADENA("sube");
				changeFrec(level);
			}
		}	
	}
}

