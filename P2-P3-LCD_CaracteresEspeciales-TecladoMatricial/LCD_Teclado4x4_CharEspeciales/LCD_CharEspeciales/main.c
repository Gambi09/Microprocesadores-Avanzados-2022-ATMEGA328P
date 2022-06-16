/*
 * LCD_CharEspeciales.c
 *
 * Created: 15/03/2022 10:59:41 a. m.
 * Author : cacho-wen
 */ 



# define F_CPU 16000000UL
# include <util/delay.h>

#include <avr/io.h>
#include <inttypes.h>
#define LCD_PUERTO PORTC //Define el Puerto al cual van a ser conectados los pines del LCD
#define LCD_RS PC0       //Define el pin del Microcontrolador al cua se conecta el pin RS del LCD
#define LCD_E PC1        //Define el pin del Microcontrolador al cual se conecta el pin E del LCD
#define LCD_DPin  DDRC
#define KEY_PIN		PIND
#define KEY_PRT 	PORTD
#define KEY_DDR		DDRD
unsigned char keypad[4][4] = {	
{'7','8','9','/'},
{'4','5','6','*'},
{'1','2','3','-'},
{' ','0','=','+'}};

unsigned char colloc, rowloc;


//Prototipos de función para LCD en modo de 4 bits
void LCD_INICIALIZA();      //Inicializa el LCD
void ENVIA_CMD(char dato);  //Función para envío de comandos
void ENVIA_DATO(char dato); // Función para envío de Datos
void ENVIA_CADENA(const char *dato);
void LIMPIA_LCD();
void CURSOR_DER();
void CURSOR_DER_CORRE();
void CURSOR_IZQ();
void CURSOR_IZQ_CORRE();
void REGRESO_CASA();
void POS_LINEA1(char posicion);
void POS_LINEA2(char posicion);
void POS_LINEA3(char posicion);
void POS_LINEA4(char posicion);
void CAR_ESP0();
void CAR_ESP1();
void animacion();

void COLUMNA_init(void);
void FILA_init(void);
unsigned char LEER_TECLADO(void);

void CreateCustomChar(unsigned char * Pattern, const char Location);

unsigned char pattern0[] = {0b00011111,0b00010101,0b00010101,0b00011111,0b00001010,0b00010101,0b00011111,0b00000000};	
unsigned char pattern1[] = {0X00, 0X15, 0X15, 0X15,0X15,0X15,0X15,0X15};
unsigned char pattern2[] = {0b00000000,0b00010001,0b00011111,0b00011111,0b00011111,0b00010001,0b00010001,0b00010001};
unsigned char pattern3[] = {0X00, 0X1F,0X11,0X11,0X15, 0X11,0X1F,0X1F};

	
unsigned char pattern4[] = {0x00,
	0x0A,
	0x0A,
	0x00,
	0x11,
	0x11,
	0x1F,
	0x0E};   //persona	
unsigned char pattern5[] = {0x04,0x04,0x0E,0x0E,0x1F,0x1D,0x1B,0x0E};	//pera
unsigned char pattern6[] = {0x00,
	0x00,
	0x1B,
	0x1F,
	0x1F,
	0x0E,
	0x04,
	0x00};	//llama
unsigned char pattern7[] = {0x11,0x1B,0x1B,0x1F,0x15,0x1F,0x0E,0x04};	//zorro
	
unsigned char rain1[] = {0x07,
	0x0F,
	0x1F,
	0x18,
	0x1F,
	0x1F,
	0x0F,
	0x07};	//lluvia1
unsigned char bat1[]={0x04,
	0x07,
	0x1F,
	0x1F,
	0x1F,
	0x1F,
	0x0F,
	0x07};
unsigned char bat2[]={0x04,
	0x1C,
	0x1F,
	0x1F,
	0x1F,
	0x1F,
	0x1E,
	0x1C};
unsigned char bat3[]={0x00,
	0x00,
	0x1F,
	0x0F,
	0x07,
	0x03,
	0x00,
	0x00};
unsigned char bat4[]={0x00,
	0x00,
	0x1F,
	0x1E,
	0x1C,
	0x18,
	0x00,
	0x00};	

void CreateCustomChar(unsigned char * Pattern, const char Location){
	int i=0;
	ENVIA_CMD(0x40+(Location*8));
	
	for (i=0; i<8; i++){
	ENVIA_DATO(Pattern[i]);	
	}
}


void LCD_INICIALIZA()
{
	LCD_DPin = 0xFF;		//Control LCD Pins (D4-D7)
	_delay_ms(15);
	ENVIA_CMD(0x02);
	ENVIA_CMD(0x28); // 00101000 DL = 0 comunicacion de 4 bits N = 1 F = 0
	ENVIA_CMD(0x0C); // D=1 Display ON Cursor OFF
	ENVIA_CMD(0x06); // I/D = 1 Incremento Automatico S=0
	ENVIA_CMD(0x01);  // Limpia LCD
	ENVIA_CMD(0X80);
}

void ENVIA_CMD(char dato)
{
	char cmd_enviar;
	cmd_enviar =(dato & 0xF0);      // se envia el nibble alto
	LCD_PUERTO =  cmd_enviar>>2;       // al puerto
	LCD_PUERTO &=~(1<<LCD_RS);  //  RS = 0
	LCD_PUERTO |= (1<<LCD_E);   //   E = 1
	_delay_ms(1);
	LCD_PUERTO &=~(1<<LCD_E);   //   E = 0
	_delay_ms(10);

	cmd_enviar=((dato<<2) & 0x3C);  // Se envia el nibble bajo
	LCD_PUERTO =  cmd_enviar;        // al puerto
	LCD_PUERTO &=~(1<<LCD_RS);   //  RS = 0
	LCD_PUERTO |= (1<<LCD_E);    //   E = 1
	_delay_ms(1);
	LCD_PUERTO &=~(1<<LCD_E);    //    E = 0
	_delay_ms(10);
	return;
}

void ENVIA_DATO(char dato)
{
	char dto_enviar;
	dto_enviar=(dato & 0xF0);      // se envia el nibble alto
	LCD_PUERTO  =  dto_enviar>>2;       // al puerto
	LCD_PUERTO |= (1<<LCD_RS)|(1<<LCD_E);  //  RS = 1   E = 1
	_delay_ms(1);
	LCD_PUERTO &=~(1<<LCD_E);//E=0
	LCD_PUERTO &=~(1<<LCD_RS);//RS=0
	_delay_ms(10);
	dto_enviar=((dato<<2) & 0x3C);  // Se envia el nibble bajo
	LCD_PUERTO =  dto_enviar;        // al puerto
	LCD_PUERTO |= (1<<LCD_RS)|(1<<LCD_E);  //  RS = 1   E = 1
	_delay_ms(1);
	LCD_PUERTO &=~(1<<LCD_E);//E=0
	LCD_PUERTO &=~(1<<LCD_RS);//RS=0
	_delay_ms(10);
	return;
}

void ENVIA_CADENA(const char *dato)
{
	int i;
	for(i=0;dato[i]!='\0';i++)
	ENVIA_DATO(dato[i]);
}

void LIMPIA_LCD()
{
	ENVIA_CMD(0x1);   //0000 0001
}

void REGRESO_CASA()
{
	ENVIA_CMD(0x02);  //0000 0010
}
void CURSOR_DER()
{
	ENVIA_CMD(0x1C);  //0001 1100
}

void CURSOR_DER_CORRE() //sin desplazar pantalla
{
	ENVIA_CMD(0x14);  //0001 1100
}

void CURSOR_IZQ()
{
	ENVIA_CMD(0x18);  // 0001 1000
}

void CURSOR_IZQ_CORRE() //Sin desplazar pantalla
{
	ENVIA_CMD(0x10);  //0001 1100
}


void POS_LINEA1(char posicion)
{
	char linea1 = 0x80;
	posicion= posicion|linea1;
	ENVIA_CMD(posicion);
}
void POS_LINEA2(char posicion)
{
	char linea2 = 0xC0;
	posicion= posicion|linea2;
	ENVIA_CMD(posicion);
}
void POS_LINEA3(char posicion)
{
	char linea3 = 0x94;
	posicion= posicion|linea3;
	ENVIA_CMD(posicion);
}
void POS_LINEA4(char posicion)
{
	char linea4 = 0xD4;
	posicion= posicion|linea4;
	ENVIA_CMD(posicion);
}

void animacion(){
	ENVIA_DATO(2);
	CURSOR_IZQ_CORRE();
	_delay_ms(50);
	ENVIA_DATO(3);
	CURSOR_IZQ_CORRE();
	_delay_ms(50);
}

void myfunction(const char *dato){
	
	int i;
	for(i=1;dato[i]!='\0';i++)
	ENVIA_DATO(dato[i]);
	_delay_ms(500);
	POS_LINEA2(0);
	for(i=0;dato[i]!='\0';i++)
	ENVIA_DATO(dato[i]);
	_delay_ms(500);
	POS_LINEA2(0);
	
}
void PARPA()
{
	ENVIA_CMD(0x0F);   //0000 1111
}



char keyfind()
{
	while(1)
	{
		DDRD = 0xF0;           /* configuramos puerto input-output */
		PORTD = 0xFF;

		do
		{
			PORTD &= 0x0F;      /* macara PORD para column read only */
			asm("NOP");
			colloc = (KEY_PIN & 0x0F); /* leyendo columna */
		}while(colloc != 0x0F);
		
		do
		{
			do
			{
				_delay_ms(20);             /* 20ms  */
				colloc = (KEY_PIN & 0x0F); /* leyendo columna */
				}while(colloc == 0x0F);        /* buscando presion de un boton */
				
				_delay_ms (40);	            /* 20 ms */
				colloc = (KEY_PIN & 0x0F);
			}while(colloc == 0x0F);

			/* now check for rows */
			PORTD = 0xEF;            /* check for pressed key in 1st row */
			asm("NOP");
			colloc = (KEY_PIN & 0x0F);
			if(colloc != 0x0F)
			{
				rowloc = 0;
				break;
			}

			KEY_PRT = 0xDF;		/* han presionado en 2nd row? */
			asm("NOP");
			colloc = (KEY_PIN & 0x0F);
			if(colloc != 0x0F)
			{
				rowloc = 1;
				break;
			}
			
			KEY_PRT = 0xBF;		/* han presionado en 3nd row? */
			asm("NOP");
			colloc = (KEY_PIN & 0x0F);
			if(colloc != 0x0F)
			{
				rowloc = 2;
				break;
			}

			KEY_PRT = 0x7F;		/* han presionado en 4th row? */
			asm("NOP");
			colloc = (KEY_PIN & 0x0F);
			if(colloc != 0x0F)
			{
				rowloc = 3;
				break;
			}
		}

		if(colloc == 0x0E)
		return(keypad[rowloc][0]);
		else if(colloc == 0x0D)
		return(keypad[rowloc][1]);
		else if(colloc == 0x0B)
		return(keypad[rowloc][2]);
		else
		return(keypad[rowloc][3]);
	}




int main(void)

{
	
	char a;
	LCD_INICIALIZA();
	REGRESO_CASA();
	CURSOR_DER();
	
	CreateCustomChar(pattern0,0);
	CreateCustomChar(pattern1,1);
	CreateCustomChar(pattern2,2);
	CreateCustomChar(pattern3,3);
	
	CreateCustomChar(pattern4,4);
	CreateCustomChar(pattern5,5);
	CreateCustomChar(pattern6,6);
	CreateCustomChar(pattern7,7);

	DDRB =0XFF;
	
	
    /* Replace with your application code */
    while (1) 
    {
		
		
		//LCD_INICIALIZA();
		//CAR_ESP0();
		//a = LEER_TECLADO();
		
		char op = keyfind();
		
		switch(op){
			case '0':
			LIMPIA_LCD();
			CreateCustomChar(pattern0,0);
			ENVIA_CMD(0x86);
			ENVIA_DATO(0);
			POS_LINEA2(3);
			ENVIA_CADENA("0 - P");
			//_delay_ms(200);
			break;
			case '1':
			LIMPIA_LCD();
			ENVIA_CMD(0x86);
			ENVIA_DATO(1);
			POS_LINEA2(3);
			ENVIA_CADENA("1 - P");
			//_delay_ms(200);
			break;
			case '2':
			LIMPIA_LCD();
			ENVIA_CMD(0x86);
			ENVIA_DATO(2);
			POS_LINEA2(3);
			ENVIA_CADENA("2 - P");
			//_delay_ms(200);
			break;
			case '3':
			LIMPIA_LCD();
			ENVIA_CMD(0x86);
			ENVIA_DATO(3);
			POS_LINEA2(3);
			ENVIA_CADENA("3 - P");
			//_delay_ms(200);
			break;
			case '4':
			LIMPIA_LCD();
			ENVIA_CMD(0x86);
			ENVIA_DATO(4);
			POS_LINEA2(3);
			ENVIA_CADENA("4 - J");
			//_delay_ms(200);
			break;
			case '5':
			LIMPIA_LCD();
			ENVIA_CMD(0x86);
			ENVIA_DATO(5);
			POS_LINEA2(3);
			ENVIA_CADENA("5 - J");
			//_delay_ms(200);
			break;
			case '6':
			LIMPIA_LCD();
			ENVIA_CMD(0x86);
			ENVIA_DATO(6);
			POS_LINEA2(3);
			ENVIA_CADENA("6 - J");
			//_delay_ms(200);
			break;
			case '7':
			LIMPIA_LCD();
			ENVIA_CMD(0x86);
			ENVIA_DATO(7);
			POS_LINEA2(3);
			ENVIA_CADENA("7 - J");
			//_delay_ms(200);
			break;
			case '8':
			LIMPIA_LCD();
			POS_LINEA2(3);
			ENVIA_CADENA("8");
			ENVIA_DATO(0);
			CreateCustomChar(rain1,0);
			
			ENVIA_CMD(0x86);
			ENVIA_DATO(0);
			ENVIA_CADENA("HEY ");
			//_delay_ms(200);
			break;
			case '9':
			LIMPIA_LCD();
			CURSOR_DER();
			ENVIA_DATO(0);
			POS_LINEA1(1);
			ENVIA_DATO('9');
			POS_LINEA2(1);
			ENVIA_CADENA("Jazmin Flores");
			
			break;
			
			case '+':
			LIMPIA_LCD();
			CURSOR_DER();
			
			POS_LINEA2(4);
			ENVIA_DATO(4);
			ENVIA_DATO(2);
			
			ENVIA_DATO(3);
			
			ENVIA_DATO(1);
			
			CreateCustomChar(bat1,2);
			CreateCustomChar(bat2,3);
			CreateCustomChar(bat3,4);
			CreateCustomChar(bat4,1);
			
			
			
			
			//POS_LINEA2(1);
			//ENVIA_CADENA("Es +");
			
			break;
			
			case '-':
			CreateCustomChar(pattern0,0);
			CreateCustomChar(pattern1,1);
			CreateCustomChar(pattern2,2);
			CreateCustomChar(pattern3,3);
			
			CreateCustomChar(pattern4,4);
			CreateCustomChar(pattern5,5);
			CreateCustomChar(pattern6,6);
			CreateCustomChar(pattern7,7);
			//_delay_ms(200);
			break;
			
			
		}

		
			

		
		
    }
}

