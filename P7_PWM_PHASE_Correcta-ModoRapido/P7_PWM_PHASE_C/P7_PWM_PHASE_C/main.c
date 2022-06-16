/*
 * P7_PWM_FAST_MODE.c
 *
 * Created: 09/06/2022 06:35:22 p. m.
 * Author : cacho-wen
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include <util/delay.h>

int step = 10;
int _freq = 0;
int limit = 255;
int a;
int x;
char str[16];

int main(void)
{
	DDRD  = 0xF0;						// ENTRADAS - INT0 Y 1
	PORTD = 0x0F;						// Pull up
	DDRC  = 0xFF;						// LCD
	LCD_INICIALIZA();
	_delay_ms(20);
	
	// Int interruptions
	cli();								// CLEAR INTERRUPTIONS
	
	PWM_init(100);
	EICRA  |= (1<<ISC01)|(1<<ISC01); // INT0 -> Flanco de subida
	EICRA  |= (1<<ISC11)|(1<<ISC10); // INT1 -> Flanco de subida
	EIMSK  |= (1<<INT0) |(1<<INT1);
	
	sei();	
								// ENABLE INTERRUPTIONS
	x = 20;
	PWM_setDutyA(20); //PWM MODO RAPIDO  TIMER1
	PWM_setDutyB(30); //PWM MODO RAPIDO  TIMER1
	PWM_setDutyC(20); //PWM FASE CORRECTA TIMER0
	PWM_on();
		
	
	while(1)
	{
		//TODO:: Please write your application code
	}
}



ISR(INT0_vect){
	// Add
	
	
	
	if ( (x+10) <= 100){
		
		x = x+10;
		PWM_SET_DUTY(x);
	}
	
	a = x;
	
	itoa(a, str, 10);
	
	ENVIA_CADENA(str);
	ENVIA_CADENA(" % duty +");
	_delay_ms(1000);
	LIMPIA_LCD();
	
	
};


ISR(INT1_vect){
	// Add
		
	
	if (x > 10){
		x = x-10;	
		PWM_SET_DUTY(x); 
	}
	
	a = x;
	itoa(a, str, 10);
	ENVIA_CADENA(str);
	ENVIA_CADENA("  % duty -");
	_delay_ms(1000);
	LIMPIA_LCD();

};


void PWM_init(int freq)
{
	//Operation Mode = Fast PWM
	TCCR1A &=~ (1<<WGM10);
	TCCR1A |=  (1<<WGM11);
	TCCR1B |=  (1<<WGM12);
	TCCR1B |=  (1<<WGM13);
	
	TCCR0A |= (1<<WGM00);
	TCCR0A &=~ (1<<WGM01);
	TCCR0B &=~ (1<<WGM02);
	
	//Compare value
	_freq = freq;
	ICR1 = (F_CPU/1024/_freq) - 1;
	
	}

void PWM_on()
{
	//PWM TIMER1
	TCNT1 = 0x0000;
	//N = 1024
	TCCR1B |=  (1<<CS10);
	TCCR1B &=~ (1<<CS11);
	TCCR1B |=  (1<<CS12);
	
	//PWM TIMER0
	TCNT0 = 0X00;
	//N = 255
	TCCR0B |= (1<<CS00);
	TCCR0B &=~(1<<CS01);
	TCCR0B |= (1<<CS02);
}

void PWM_off()
{
	//Clock setting T1clock = 0 Hz
	TCCR1B &=~ (1<<CS10);
	TCCR1B &=~ (1<<CS11);
	TCCR1B &=~ (1<<CS12);
	
	TCCR0B &=~ (1<<CS00);
	TCCR0B &=~ (1<<CS01);
	TCCR0B &=~ (1<<CS02);
}

void PWM_setDutyA(int duty)
{
	//Pin configuration
	DDRB |= (1<<DDB1);
	
	//Output active
	TCCR1A &=~ (1<<COM1A0);
	TCCR1A |=  (1<<COM1A1);

	OCR1A = (((F_CPU/1024/_freq) - 1)*duty)/100;
}

void PWM_setDutyB(int duty)
{
	//Pin configuration
	DDRB |= (1<<DDB2);

	//Output active
	TCCR1A &=~ (1<<COM1B0);
	TCCR1A |=  (1<<COM1B1);

	OCR1B = (((F_CPU/1024/_freq) - 1)*duty)/100;
}

void PWM_setDutyC(int duty)
{
	//Pin configuration
	DDRD |= (1<<DDD6);
	
	//Output active
	TCCR0A |= (0<<COM0B0);
	TCCR0A |= (1<<COM0B1);

	OCR0B = (255*duty)/100;
}

void PWM_SET_DUTY(int d)
{
	OCR1B = (((F_CPU/1024/(100))-1)*d)/100;
	OCR0B = (d*255)/100;
}