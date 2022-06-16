/*
 * Practica1328p.c
 *
 * Created: 21/02/2022 05:57:29 p. m.
 * Author : cacho-wen
 * Programa para prender y apagar puertos del ATMEGA 328P
 */ 

/*Biblioteca no tienen un principal vs Ejecutable el ejecutable tiene un principal
Ejecutable: solo se utiliza una vez
Biblioteca: Cuando se manda a llamar mas de una vez en el programa, asi reducimos el main principal   
*/
//1ro Comentarios
//2do Define Include/bibliotecas: .h 
# define F_CPU 16000000UL //Nos dice a que vel va el CPU, para utilizar delay, si no se establece 
//queda en 1MHZ por defecto

# include <util/delay.h> //#include <>  es una bibllioteca dentro del compilador
						 //#include " " bibliotecas que nosotros hicimos
# include <avr/io.h>

//3ro Variables globales
int i=0;
	
//10101010
int A = 0XAA;//0b10101010; //D=170 H=AA
int B = 0X55;//0b01010101; //D=85  H=55
int C = 0X00;

//4to Cabeceras de funiones

//5to Main Principal: Debe ser lo mas corto que se pueda
int main(void)  //PRINCIPAL
{
    /* Replace with your application code */
	// CONFIGURACION DE LA MAQUINA, OSEA DEL MICRO Xd hasta ahorita le capte 
	//DORC=0, _____0b0-BINARIO, 0 Decimal, 0X00 Hexadecimal
	 
	
	DDRB = 0XFF; //PARA ESTABLECER TIPO I/O 0=I
	DDRD = 0XFF; 
	
	
	
	
	
    while (1) 
    {
			PORTB = A; // ESTADO DEL PUERTO 
			PORTD = B;
			_delay_ms(500);
			PORTB = B;
			PORTD = A; 
			_delay_ms(500);
			PORTB = C;
			PORTD = C;
			_delay_us(500);
    }
}