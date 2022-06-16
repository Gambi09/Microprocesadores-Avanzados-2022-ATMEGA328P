/*
 * P4_Interrupciones.c
 *
 * Created: 06/04/2022 02:12:08 p. m.
 * Author : cacho-wen
 */ 

#include <avr/io.h>


# define F_CPU 16000000UL
# include <util/delay.h>

#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include "lcd.h"



unsigned char pattern0[] = {0b00011111,0b00010101,0b00010101,0b00011111,0b00001010,0b00010101,0b00011111,0b00000000};
unsigned char pattern1[] = {0X00, 0X15, 0X15, 0X15,0X15,0X15,0X15,0X15};
unsigned char pattern2[] = {0b00000000,0b00010001,0b00011111,0b00011111,0b00011111,0b00010001,0b00010001,0b00010001};
unsigned char pattern3[] = {0X00, 0X1F,0X11,0X11,0X15, 0X11,0X1F,0X1F};


unsigned char pattern4[] = {0x00,0x0A,0x0A,0x00,0x11,0x11,0x1F,0x0E};   //sonrisa
unsigned char pattern5[] = {0x0E,0x1B,0x1B,0x0E,0x04,0x06,0x04,0x04};	//flor
unsigned char pattern6[] = {0x00,0x00,0x1B,0x1F,0x1F,0x0E,0x04,0x00};	//corazon
unsigned char pattern7[] = {0x11,0x1B,0x1B,0x1F,0x15,0x1F,0x0E,0x04};	//zorro

unsigned char rain1[] =    {0x07,0x0F,0x1F,0x18,0x1F,0x1F,0x0F,0x07};	//lluvia1
unsigned char bat1[]  =	   {0x04,0x07,0x1F,0x1F,0x1F,0x1F,0x0F,0x07};
unsigned char bat2[]  =    {0x04,0x1C,0x1F,0x1F,0x1F,0x1F,0x1E,0x1C};
unsigned char bat3[]  =    {0x00,0x00,0x1F,0x0F,0x07,0x03,0x00,0x00};
unsigned char bat4[]  =    {0x00,0x00,0x1F,0x1E,0x1C,0x18,0x00,0x00};



int i=0, j=1, bandera=0, a=0x0F, b=0xAA, c= 0x00;
ISR (INT0_vect) {
		LIMPIA_LCD();
		ENVIA_CMD(0x86);
		ENVIA_DATO(4);
		ENVIA_DATO(4);
		ENVIA_DATO(4);
		POS_LINEA2(3);
		ENVIA_CADENA("Jazmin");
		ENVIA_DATO(' ');
		ENVIA_DATO(6);
		ENVIA_DATO(6);
		ENVIA_DATO(6);
		
		_delay_ms(500);
		LIMPIA_LCD();
		
		
}

ISR (INT1_vect) {
	LIMPIA_LCD();
	
	POS_LINEA1(5);
	ENVIA_CADENA("Flores");
	
	ENVIA_DATO(6);
	POS_LINEA2(0x86);
	ENVIA_DATO(5);
	ENVIA_DATO(5);
	ENVIA_DATO(5);
	ENVIA_DATO(5);
	ENVIA_DATO(5);
	_delay_ms(500);
	LIMPIA_LCD();
}

/*ISR (PCINT1_vect) {
	LIMPIA_LCD();
	ENVIA_CMD(0x86);
	ENVIA_DATO(5);
	ENVIA_DATO(5);
	ENVIA_DATO('C');
	POS_LINEA2(1);
	ENVIA_CADENA("Soy la venganza");
	_delay_ms(500);
	
}
*/
ISR (PCINT0_vect) {
	LIMPIA_LCD();
	ENVIA_CMD(0x86);
	ENVIA_DATO(7);
	ENVIA_DATO(7);
	ENVIA_DATO(7);
	POS_LINEA2(5);
	ENVIA_CADENA("Upiita");
	_delay_ms(500);	
	LIMPIA_LCD();
}

ISR (PCINT2_vect) {
	LIMPIA_LCD();
	ENVIA_CMD(0x86);
	
	ENVIA_DATO(2);
	ENVIA_DATO(0);
	ENVIA_DATO(1);
	ENVIA_DATO(3);
	
	POS_LINEA2(1);
	ENVIA_CADENA("la Venganza...");
	_delay_ms(500);
	LIMPIA_LCD();
}


int main(void)
{
	
    DDRB =0X00;
    DDRD =0X00;
    PORTB =0b00000001;
    PORTD =0b00001101;
	
	/* Replace with your application code */
    cli();
    LCD_INICIALIZA();
    REGRESO_CASA();
    CURSOR_DER();
    
    CreateCustomChar(bat1,0);
    CreateCustomChar(bat2,1);
    CreateCustomChar(bat3,2);
    CreateCustomChar(bat4,3);
    
    CreateCustomChar(pattern4,4);
    CreateCustomChar(pattern5,5);
    CreateCustomChar(pattern6,6);
    CreateCustomChar(pattern7,7);
	
	
	
    
	EIMSK |= (1<<INT0);
	EICRA |= (1<<ISC01)|(1<<ISC00);
	
	EIMSK |= (1<<INT1);
	EICRA |= (1<<ISC01)|(1<<ISC00);
    
	PCICR |= (1<<PCIE2);
	PCICR |= (0<<PCIE1);
	PCICR |= (1<<PCIE0);
	
	PCMSK0|= (1<<PCINT0);
	PCMSK2|= (1<<PCINT16);
	
	LIMPIA_LCD();
    POS_LINEA2(3);
    ENVIA_CADENA("Comienza...");
	sei();
    /* Replace with your application code */
    while (1)
    {
		
		/*int a=0;
		if (a==0){
		LIMPIA_LCD();
		POS_LINEA2(3);
		ENVIA_CADENA("Comienza...");
		a=1;
		}*/
    }
}

