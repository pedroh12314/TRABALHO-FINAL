# include <stm32f10x.h>


// ESCOPO DE FUNCOES
void buzzer( uint16_t );
void lcd_init ( void ); // Iniciar o display corretamente
void lcd_command ( unsigned char cmd ); // Enviar comandos
void lcd_data ( unsigned char data ); // Envia dados ( caractere ASCII )
void lcd_print ( char * str ); // Envia strings para o display
void lcd_putValue ( unsigned char value ); // Usada internamente
void delay_us ( uint16_t t ); // Atraso de micro segundos
void delay_ms ( uint16_t t ); // Atraso de mili segundos
void atualiza_ldc( void );

// BUZZER
# define bz 0 // PB0

// ACORDE EM USO
#define SW1 0xEF38
#define SW2 0xDF38

// CICLO DE TRABALHO
#define SW3 0xBF38
#define SW4 0x7F38

// TECLAS de tons
#define SW5 0xFF18
#define SW6 0xFF28
#define SW7 0xFF30
#define SW8 0x0090
#define SW9 0x0088
#define SW10 0xFE38
#define SW11 0xFD38
#define SW12 0xFB38
#define SW13 0xF738
#define SW14 0x0018
#define SW15 0x6000
#define SW16 0xA000
#define SW17 0xC000

// LCD
#define LCD_RS 15 // PA15
#define LCD_EN 12 // PA12
#define LCD4 8 // PA8
#define LCD5 6 // PA6
#define LCD6 5 // PA5
#define LCD7 11 // PA11	



	
int main ()
{

  RCC -> APB2ENR |= 0xFC ; // Enable the clocks for GPIO ports
  RCC -> APB2ENR |= RCC_APB2ENR_AFIOEN ;
	RCC -> APB1ENR |= (1 <<1); /* enable TIM3 clock */
	TIM3 -> CCR3 = 12;
	TIM3 -> CCER &= ~(1 <<8); // disable TIM3
	TIM3 -> CCMR2 = 0x0030 ; /* toggle channel 3 */
	TIM3 -> PSC = 7200 -1;	
	
	// Desabilita inteface JTAG
  AFIO -> MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE ;

  // GPIO setup
  GPIOA -> CRL = 0x43344333 ; // LCD pins as outputs
  GPIOA -> CRH = 0x33333443 ; // and other pins as inputs
	GPIOB -> CRL = 0x4444442B ;
  GPIOB -> CRH = 0x44444444 ;
	GPIOC -> CRL = 0x44444444 ;
  GPIOC -> CRH = 0x44444444 ;
	 
		// 4 INPUT digital
		// 3 OUTPUT digital
		
		// 2 INPUT analog - ta certo?
		// 1 OUTPUT analog
	
	// Inicia LCD 
	lcd_init ();
	delay_ms (100);
	lcd_command(0x0C); // Desliga cursor
	lcd_command(0x80);
	lcd_print("Oitava: 1");
	lcd_command(0xC0);
	lcd_print("Dutty: 25%");
	
	// Variaveis Globais
	int oitava = 1;
	int dutty = 25;
	uint16_t teclas_GPIOA;
	uint16_t teclas_GPIOB;
	uint16_t teclas_GPIOC;
	

	// LOOP
	while (1)
	{
		
		// Setup Portas LCD
		//utilizar_teclas();
		
		teclas_GPIOA = GPIOA -> IDR;
		teclas_GPIOA &= 0x0098;
		switch ( teclas_GPIOA )
			{
				case SW8 :	
						buzzer(33);
					break ;

			case SW9 :		
					buzzer(33);
					break ;
				
				case SW14 :		
					buzzer(33);
					break ;	
				
				default :
					TIM3 -> CCER =0x0000 ; // desenable sound
					TIM3 -> CR1 = 0;
					GPIOA -> ODR = 0x0000;
					break ;

			}
			
		teclas_GPIOB = GPIOB -> IDR;
		teclas_GPIOB &=0xFF38;
		switch ( teclas_GPIOB )
			{
				case SW1 :
					if(oitava == 2)
					{
						oitava = 1;
						lcd_command(0x80);
						lcd_print("Oitava: 1");
					}
					break ;
				
				case SW2 :
					if(oitava == 1)
					{
						oitava = 2;
						lcd_command(0x80);
						lcd_print("Oitava: 2");
					}
					break ;
				
				case SW3 :
					if(dutty == 25)
					{
						dutty = 50;
						lcd_command(0xC0);
						lcd_print("Dutty: 25%");
						delay_ms(800);
					}else if (dutty == 50)
					{
						dutty = 75;
						lcd_command(0xC0);
						lcd_print("Dutty: 50%");
						delay_ms(800);
					}else if (dutty == 75)
					{
						dutty = 25;
						lcd_command(0xC0);
						lcd_print("Dutty: 75%");
						delay_ms(800);
					}
					
					break ;
				
				case SW4 :		
					buzzer(33);
					break ;
				
				case SW5 :		
					buzzer(33);
					break ;
				
				case SW6 :		
					buzzer(33);
					break ;
				
				case SW7 :		
					TIM3 -> CCER =0x0000 ; // desenable sound
					TIM3 -> CR1 = 0;
					GPIOA -> ODR = 0x0000;
					break ;
				
				case SW10 :		
					buzzer(33);
					break ;		
				
				case SW11 :		
					buzzer(33);
					break ;		
				
				case SW12 :		
					buzzer(33);
					break ;
				
				case SW13 :		
					buzzer(33);
					break ;
								
				default :
					TIM3 -> CCER =0x0000 ; // desenable sound
					TIM3 -> CR1 = 0;
					GPIOA -> ODR = 0x0000;
					break ;

			}
			
		teclas_GPIOC = GPIOC -> IDR;
		teclas_GPIOC &=0xE000;
		switch ( teclas_GPIOC )
			{
				
				case SW15 :		
					buzzer(33);
					break ;
				
				case SW16 :		
					buzzer(33);
					break ;
				
				case SW17 :		
					buzzer(33);
					break ;			
				
				default :
					TIM3 -> CCER =0x0000 ; 
					TIM3 -> CR1 = 0;
					GPIOA -> ODR = 0x0000;
					break ;

			}
			
			// LCD
			//atualiza_ldc();
	
	}
}


void atualiza_ldc()
{
	lcd_print("teste "); // inserir variavel oitava
	delay_ms (3);
	
	//lcd_command (0x02/*ver*/); // go to second line
	//delay_ms (3);
	//lcd_print("Dutty: "); // inserir dutty
	//delay_ms (3);
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