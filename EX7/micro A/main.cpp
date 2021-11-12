#define F_CPU 8000000UL			/* Define frequency here its 8MHz */
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
#define enable            5
#define registerselection 6
#define LCD_CLR           0   


unsigned char scan_key(void);
unsigned char code[4][4]={ {7,4,1,10},{8,5,2,0},{9,6,3,11},{12,13,14,15} };
char lcd_buffer[12];
unsigned char stepsCount = 0;

void send_a_command(unsigned char command);
void send_a_character(unsigned char character);
void send_a_string(char *string_of_characters);
void lcd_clrscr(void);
void showTemp(void);
void UART_init(long USART_BAUDRATE);
unsigned char UART_RxChar();
void UART_TxChar(char ch);
void UART_SendString(char *str);
unsigned char scan_key(void);
void sendStepsCount();


int main(void)
{  

    unsigned char key;
    PORTC = 0x00;
    DDRC = 0xF0;
    DDRB = 0xFF;
    DDRD = 0b01100000;
    DDRA |= 0<<2;

    while (1){   
        send_a_command(0x01);
        _delay_ms(50);
        send_a_command(0x38);
        _delay_ms(50);
        send_a_command(0b00001111);
        _delay_ms(50);
        send_a_string("Admin ID: ");
        key = scan_key();
        if(key != 0xFF){
            sprintf(lcd_buffer, "%d", key);
            send_a_string(lcd_buffer);    
        }   
        _delay_ms(500);
        lcd_clrscr();
        send_a_string("Admin Pass: ");
        if(key != 0xFF){
            sprintf(lcd_buffer, "%d", key);
            send_a_string(lcd_buffer);    
        }
        lcd_clrscr();

        if(PINA2|0x00 == 1){
            send_a_string("Temp(C)= ");
            while((UCSRA&0x80)==0x00);
            showTemp();
        }
        else if(PIN2|0x00 == 0){
            sendStepsCount();
        }
    }
}


void showTemp(void){
    DDRB = 0xFF;
    DDRD = 0b01100000;
    _delay_ms(30);
    
    //int COUNTA = 0;
    char SHOWA;
     
    send_a_command(0x01); //Clear Screen 0x01 = 00000001
    _delay_ms(50);
    send_a_command(0x38);
    _delay_ms(50);
    send_a_command(0b00001111);
    _delay_ms(50);
    
    SHOWA = UART_RxChar();
    send_a_string ("Temp(C)=");
    //itoa(COUNTA,SHOWA,10);
    send_a_character(SHOWA);
    send_a_string ("      ");
    send_a_command(0x80 + 0);
    
}

//lcd functions
void send_a_command(unsigned char command)
{
    PORTB = command;
    PORTD &= ~ (1<<registerselection);
    PORTD |= 1<<enable;
    _delay_ms(20);
    PORTD &= ~1<<enable;
    PORTB = 0;
}
void send_a_character(unsigned char character)
{
    PORTB = character;
    PORTD |= 1<<registerselection;
    PORTD |= 1<<enable;
    _delay_ms(20);
    PORTD &= ~1<<enable;
    PORTB = 0;

}
void send_a_string(char *string_of_characters)
{
    while(*string_of_characters > 0)
    {
        send_a_character(*string_of_characters++);
    }
}
void lcd_clrscr(void)
{
    send_a_command(1<<LCD_CLR);
}

void UART_init(long USART_BAUDRATE)
{
	UCSRB |= (1 << RXEN) | (1 << TXEN);/* Turn on transmission and reception */
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);/* Use 8-bit character sizes */
	UBRRL = BAUD_PRESCALE;		/* Load lower 8-bits of the baud rate value */
	UBRRH = (BAUD_PRESCALE >> 8);	/* Load upper 8-bits*/
}

unsigned char UART_RxChar()
{
	while ((UCSRA & (1 << RXC)) == 0);/* Wait till data is received */
	return(UDR);			/* Return the byte*/
}

void UART_TxChar(char ch)
{
	while (! (UCSRA & (1<<UDRE)));	/* Wait for empty transmit buffer*/
	UDR = ch ;
}

void UART_SendString(char *str)
{
	unsigned char j=0;
	
	while (str[j]!=0)		/* Send string till null */
	{
		UART_TxChar(str[j]);	
		j++;
	}
}

unsigned char scan_key(void)
{
    unsigned char i,data,num_key,temp;
    num_key=0xff;
    temp=0x70;
    for(i=0;i<4;i++){
        PORTC=temp;
        _delay_ms(5);
        data=PINC & 0x0f;
        if(data==0x07)
        num_key=code[0][i];
        if(data==0x0B)
        num_key=code[1][i];
        if(data==0x0D)
        num_key=code[2][i];
        if(data==0x0E)
        num_key=code[3][i];
        temp= ((temp>>=1) | 0x80) & 0xF0 ;
    }
    return num_key;
}

void sendStepsCount(){
    stepsCount = scan_key();
    UART_TxChar(stepsCount);
}