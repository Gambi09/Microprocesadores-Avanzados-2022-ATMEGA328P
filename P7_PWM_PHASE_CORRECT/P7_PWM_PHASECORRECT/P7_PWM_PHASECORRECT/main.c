/*
 * P7_PWM_FAST_MODE.c
 *
 * Created: 09/06/2022 06:35:22 p. m.
 * Author : cacho-wen
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <C:\Users\oem\Desktop\Micros\P7_PWM_PHASE_CORRECT\P7_PWM_PHASECORRECT\P7_PWM_PHASECORRECT\lcdAB.h>
#include <util/delay.h>

int step = 10;
int limit = 255;
int a;
int x;
char str[16];

int main(void)
{
	DDRD  = 0x00;						// ENTRADAS - INT0 Y 1
	PORTD = 0xFF;						// Pull up
	DDRC  = 0xFF;						// LCD
	LCD_INICIALIZA();
	_delay_ms(20);
	
	
	// Int interruptions
	cli();								// CLEAR INTERRUPTIONS
	PWM_init(50,20);
	x = 20;
	EICRA  |= (1<<ISC01)|(1<<ISC01); // INT0 -> Flanco de subida
	EICRA  |= (1<<ISC11)|(1<<ISC10); // INT1 -> Flanco de subida
	EIMSK  |= (1<<INT0) |(1<<INT1);
	sei();								// ENABLE INTERRUPTIONS
	
	PWM_ON();	
	
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


void PWM_init(int freq, int duty)
{
								// Ctc value
	//PIN CONFIGURATION 
	DDRB    = (1<<DDB1);
	DDRB	= (1<<DDB2);
	
	// Output ACTIVE
	TCCR1A &=~  (1<<COM1A0);	
	TCCR1A  |=  (1<<COM1A1);
	
	TCCR1A  &=~  (1<<COM1B0);
	TCCR1A  |=   (1<<COM1B1);
			
	//// PWM MODE FAST
	TCCR1A	&=~  (1<<WGM10);					// START CLOCLS
	TCCR1A  |=   (1<<WGM11);
	TCCR1B  |=   (1<<WGM12);
	TCCR1B  |=   (1<<WGM13);
	
	//COMPARE VALUE
	ICR1 = (F_CPU/1024/freq) -1;
	OCR1A = (((F_CPU/1024/freq)-1)*duty)/100;
	OCR1B = (((F_CPU/1024/freq)-1)*duty)/100;
}

void PWM_ON()
{
	TCNT1 = 0X0000;
	TCCR1B |= (1<<CS10);
	TCCR1B &=~ (1<<CS11);
	TCCR1B |= (1<<CS12);
}

void PWM_OFF()
{
	
	TCCR1B &=~ (1<<CS10);
	TCCR1B &=~ (1<<CS11);
	TCCR1B &=~ (1<<CS12);
}

void PWM_SET_DUTY(int d)
{
	OCR1B = (((F_CPU/1024/(50))-1)*d)/100;
}