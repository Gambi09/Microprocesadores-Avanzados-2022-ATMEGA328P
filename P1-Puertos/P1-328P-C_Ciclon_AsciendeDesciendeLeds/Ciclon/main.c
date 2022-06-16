/*
 * Ciclon.c
 *
 * Created: 01/03/2022 10:14:51 p. m.
 * Author : cacho-wen
 */ 

# define F_CPU 16000000UL //Nos dice a que vel va el CPU, para utilizar delay, si no se establece
//queda en 1MHZ por defecto

# include <util/delay.h> //#include <>  es una bibllioteca dentro del compilador
//#include " " bibliotecas que nosotros hicimos
# include <avr/io.h>
int x = 0;
int b = 0;
int d = 1;

int main(void)
{
    /* Replace with your application code */
	DDRB = 0XFF; //PARA ESTABLECER TIPO I/O 0=I
	DDRD = 0XFF;
	
	
	
    while (1) 
    {
		if (d==1){
			for (x=0;x<=7;x++){
				PORTD = d;
				d=d*2;
				_delay_ms(500);
			}
		}
		else if (d>127){
			
			for (b=8;b>0;b--){
				d=d/2;
				PORTD = d;
				_delay_ms(500);
			}
		}
		
    }
}

