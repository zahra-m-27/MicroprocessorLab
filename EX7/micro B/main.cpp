#include <avr/io.h>
#define F_CPU 8000000UL			/* Define frequency here its 8MHz */
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


unsigned char getTemp(void);
unsigned char sendTemp(void);
void rotateStepperMotor(void);
void UART_init(long USART_BAUDRATE);
unsigned char UART_RxChar();
void UART_TxChar(char ch);
void UART_SendString(char *str);

int main(void)
{   
    unsigned char key;
    DDRC |= 0<<2 ;
    DDRB = 0xFF;
    DDRD = 0b01100000;
    DDRA = 0;

    while (1){
        if(PINC2|0x00 == 1){
            UDR = sendTemp();
            while((UCSRA&0x80)==0x00);
        }
        else if(PINC2|0x00 == 0){
            rotateStepperMotor();
        }
    }
}


unsigned char getTemp(void){
    DDRA = 0;
    _delay_ms(30);
    
    ADMUX |=(1<<REFS0)|(1<<REFS1);
    ADCSRA |=(1<<ADEN)|(1<<ADATE)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
    
    unsigned char COUNTA = 0;     
    COUNTA = ADCW/4;
    ADCSRA |=(1<<ADSC);       
    //ADCSRA |=(1<<ADSC);

    return COUNTA;    
       
}

unsigned char sendTemp(void){
    unsigned char temp = 0;
    char SHOWA;

    UBRRH = 0x00;
    UBRRL = 51;
    UCSRA = 0x00;
    UCSRB = 0x08;
    UCSRC = 0x86; 
    _delay_ms(1000);
    temp = getTemp();
    //itoa(temp,SHOWA,10);
    UART_TxChar(temp); 
}

void rotateStepperMotor(void){

    unsigned char stepCount = 0;

    #define a1 (1<<PB4)
    #define a2 (1<<PB5)
    #define a3 (1<<PB6)
    #define a4 (1<<PB7)

	DDRB |= a1 | a2 | a3| a4;
    stepCount = UART_RxChar();

	while(stepCount>0){

		PORTB |= a1;
		PORTB &= ~a2;
		PORTB &= ~a3;
		PORTB |= a4;
		_delay_ms(100);

		PORTB |= a1;
		PORTB &= ~a2;
		PORTB |= a3;
	    PORTB &= ~a4;
		_delay_ms(100);

		PORTB &= ~a1;
		PORTB |= a2;
		PORTB |= a3;
		PORTB &= ~a4;
		_delay_ms(100);

		PORTB &= ~a1;
		PORTB |= a2;
		PORTB &= ~a3;
		PORTB |= a4;
		_delay_ms(100);	

        stepCount--;
	}

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