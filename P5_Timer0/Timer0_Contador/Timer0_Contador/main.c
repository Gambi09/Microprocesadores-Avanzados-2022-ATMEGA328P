/*
 * Timer0_Contador.c
 *
 * Created: 06/04/2022 06:54:41 p. m.
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

int a;
char str[16];

ISR (TIMER0_OVF_vect) 
{
	LIMPIA_LCD();
	POS_LINEA1(0);
	ENVIA_CADENA("Si, llegamos a  10");
	POS_LINEA2(0);
	ENVIA_CADENA("esperemos otro lote");
	
	}

ISR (INT1_vect) {
		a = TCNT0;
		itoa(a, str, 10);
		LIMPIA_LCD();
		POS_LINEA1(0);
		ENVIA_CADENA("Bienvenido");
		POS_LINEA2(3);
		ENVIA_CADENA("cliente ");
		ENVIA_CADENA(str);
		_delay_ms(500);
	LIMPIA_LCD();
	POS_LINEA1(3);
	ENVIA_CADENA(str);
	POS_LINEA2(3);
	ENVIA_CADENA("OPEN");
}

int main(void)
{
    /* Replace with your application code */
	cli();
	LCD_INICIALIZA();
	REGRESO_CASA();
	CURSOR_DER();
	//PD4 ENTRA CLK EXTERNA()
	//CONTAMOS CUANTAS PERSONAS ENTRAN EN EL LCD
	DDRD = 0XF0;
	PORTD = 0b00011000;
	//External T0 pin Clock on falling edge
	TCCR0B = (1<<CS02)|(1<<CS01)|(0<<CS00);
	//TCCR0B = (0<<WGM00)|(1<<WGM01);
	TIMSK0 = (1<<TOIE0);
	 
	 
	OCR0B = 2;
	TCNT0 = 0;
	
	EIMSK |= (1<<INT1);
	EICRA |= (1<<ISC01)|(1<<ISC00);
	 
	sei();
	
    LIMPIA_LCD();
    POS_LINEA2(3);
    ENVIA_DATO('O');
    ENVIA_DATO('P');
    ENVIA_DATO('E');
    ENVIA_DATO('N');
	
	while (1) 
    {
	
	}
    }

