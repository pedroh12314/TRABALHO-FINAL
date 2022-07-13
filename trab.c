# include <stm32f10x.h>


// Setup funcoes


void buzzer( uint16_t );
void lcd_init ( void ); // Iniciar o display corretamente
void lcd_command ( unsigned char cmd ); // Enviar comandos
void lcd_data ( unsigned char data ); // Envia dados ( caractere ASCII )
void lcd_print ( char * str ); // Envia strings
void lcd_putValue ( unsigned char value ); // Usada internamente
void delay_us ( uint16_t t ); // Atraso de micro segundos
void delay_ms ( uint16_t t ); // Atraso de mili segundos
void atualiza_ldc( void );
	

// Setup Chaves

// BUZZER
# define bz 0 // PB0

// ACORDE EM USO
#define SW1 0x0000EFF0
#define SW2 0x0000DFF0

// CICLO DE TRABALHO
#define SW3 0x0000BFF0
#define SW4 0x00007FF0

// TONS
#define SW5 0x0000FFD0
#define SW6 0x0000FFE0
//#define SW7 0x0000FFF0
//#define SW8 0x00007FF8
//#define SW9 0x0000EFF8
#define SW10 0x0000FEF0
#define SW11 0x0000FDF0
#define SW12 0x0000F7F0
#define SW13 0x0000FBF0
//#define SW14 0x0000EFF0
//#define SW15 0x0000DFF0
//#define SW16 0x0000BFF8
//#define SW17 0x00007FF8

// LCD
#define LCD_RS 15 // PA15
#define LCD_EN 12 // PA12
#define LCD4 8 // PA8
#define LCD5 6 // PA6
#define LCD6 5 // PA5
#define LCD7 11 // PA11	
	
int main ()
{
	AFIO -> MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE ;
	// Setup Buzzer
	RCC -> APB2ENR |= 0xFC ; /* enable GPIO clocks */
	RCC -> APB1ENR |= (1 <<1); /* enable TIM3 clock */
	GPIOB -> CRL = 0x4444444B ; // config buzzer as alt . func .
	GPIOB -> CRH = 0x44444444 ; // config switches as inputs
	TIM3 -> CCR3 = 12;
	TIM3 -> CCER &= ~(1 <<8); // disable TIM3
	TIM3 -> CCMR2 = 0x0030 ; /* toggle channel 3 */
	TIM3 -> PSC = 7200 -1;
	// Setup LCD
	GPIOA -> CRL = 0x43344444 ; // LCD pins as outputs
  GPIOA -> CRH = 0x34433443 ; // and other pins as inputs
	
	// Inicia LCD 
	lcd_init ();
	delay_ms (100);
	
	
	// Variaveis Globais
	int oitava = 1;
	
	
	// LOOP
	while (1)
	{
		switch ( GPIOB -> IDR )
			{
				case SW1 :
					oitava = 1;
					break ;
				
				case SW2 :
					oitava = 2;
					break ;
				
				case SW3 :
					//buzzer
					buzzer(55);
					break ;
				
				case SW4 :		
					//buzzer
					buzzer(33);
					break ;
				
				case SW5 :		
					//buzzer
					buzzer(33);
					break ;
				
				case SW6 :		
					//buzzer
					buzzer(33);
					break ;
				
				//case SW7 :		
					//buzzer
					//buzzer(33);
					//break ;
				
				//case SW8 :		
					//buzzer
					//buzzer(33);
					//break ;
				
				//case SW9 :		
					//buzzer
					//buzzer(33);
					//break ;				
				
				case SW10 :		
					//buzzer
					buzzer(33);
					break ;		
				
				case SW11 :		
					//buzzer
					buzzer(33);
					break ;		
				
				case SW12 :		
					//buzzer
					buzzer(33);
					break ;
				
				case SW13 :		
					//buzzer
					buzzer(33);
					break ;
				
				//case SW14 :		
					//buzzer
					//buzzer(33);
					//break ;
				
				//case SW15 :		
					//buzzer
					//buzzer(33);
					//break ;
				
			//	case SW16 :		
					//buzzer
				//	buzzer(33);
				//	break ;
				
			//	case SW17 :		
					//buzzer
				//	buzzer(33);
				//	break ;				
				default :
					TIM3 -> CCER =0x0000 ; // desenable sound
					TIM3 -> CR1 = 0;
					GPIOA -> ODR = 0x0000;
					break ;

			}
			

			atualiza_ldc();
	
	}
}

void atualiza_ldc()
{
	lcd_command (0x02); // return to first line
	delay_ms (3);
	lcd_data(2);

}

void buzzer(uint16_t f)
{
					TIM3 -> CCER |=(1 <<8); // enable sound
					TIM3 -> ARR = f;
					TIM3 -> CR1 = 1;
					delay_ms (100);
}


void lcd_command(unsigned char cmd)
{
		GPIOA->ODR &= ~ (1<<LCD_RS); /* RS = 0 for command */
		lcd_putValue(cmd);
}

void lcd_data(unsigned char data)
{
		GPIOA->ODR |= (1<<LCD_RS); /* RS = 1 for data */
		lcd_putValue(data);
}

void lcd_print(char * str)
{
		unsigned char i = 0;
		while(str[i] != 0) /* while it is not end of string */
		{
		lcd_data(str[i]); /* show str[i] on the LCD */
		i++;
		}
}
		
void lcd_init()
{
		delay_ms(15);
		GPIOA->ODR &= ~(1<<LCD_EN); /* LCD_EN = 0 */
		delay_ms(3); /* wait 3ms */
		lcd_command(0x33); //lcd init.
		delay_ms(5);
		lcd_command(0x32); //lcd init.
		delay_us(3000);
		lcd_command(0x28); // 4-bit mode, 1 line and 5x8 charactere set
		delay_ms(3);
		lcd_command(0x0e); // display on, cursor on
		delay_ms(3);
		lcd_command(0x01); // display clear
		delay_ms(3);
		lcd_command(0x06); // move right
		delay_ms(3);
}
		
void delay_us(uint16_t t)
{
		volatile unsigned long l = 0;
		for(uint16_t i = 0; i < t; i++)
		for(l = 0; l < 6; l++)
		{}
}

void delay_ms(uint16_t t)
{
		volatile unsigned long l = 0;
		for(uint16_t i = 0; i < t; i++)
		for(l = 0; l < 6000; l++)
		{}
}

void lcd_putValue(unsigned char value)
{
		uint16_t aux; //variable to help to build appropriate data out
		aux = 0x0000; //clear aux
		GPIOA->BRR = (1<<5)|(1<<6)|(1<<8)|(1<<11); // clear data lines
		aux = value & 0xF0;
		aux = aux>>4;
		GPIOA->BSRR = ((aux&0x0008)<<8) | ((aux&0x0004)<<3) |	((aux&0x0002)<<5) | ((aux&0x0001)<<8);
		GPIOA->ODR |= (1<<LCD_EN); /* EN = 1 for H-to-L pulse */
		delay_ms(3); /* make EN pulse wider */
		GPIOA->ODR &= ~ (1<<LCD_EN); /* EN = 0 for H-to-L pulse */
		delay_ms(1); /* wait */
		GPIOA->BRR = (1<<5)|(1<<6)|(1<<8)|(1<<11); // clear data lines
		aux = 0x0000; //clear aux
		aux = value & 0x0F;
		GPIOA->BSRR = ((aux&0x0008)<<8) | ((aux&0x0004)<<3) | ((aux&0x0002)<<5) | ((aux&0x0001)<<8);
		GPIOA->ODR |= (1<<LCD_EN); /* EN = 1 for H-to-L pulse */
		delay_ms(3); /* make EN pulse wider */
		GPIOA->ODR &= ~(1<<LCD_EN); /* EN = 0 for H-to-L pulse */
		delay_ms(1); /* wait */
}