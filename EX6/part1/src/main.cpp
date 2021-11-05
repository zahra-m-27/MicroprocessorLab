#include <avr/io.h>
#define F_CPU 1000000
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>


#define enable            5
#define registerselection 6
bool po = 0;

void send_a_command(unsigned char command);
void send_a_character(unsigned char character);
void send_a_string(char *string_of_characters);

ISR(INT0_vect){
    po = !po;
}

int main(void)
{
    DDRB = 0xFF;
    DDRA = 0;
    DDRD = 0b01100000;
    MCUCR = 0x01;
    GICR = 0x40;
    GIFR = 0x40;
    sei();
    _delay_ms(30);
    
    ADMUX |=(1<<REFS0)|(1<<REFS1);
    ADCSRA |=(1<<ADEN)|(1<<ADATE)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
    
    float COUNTA = 0;
    char SHOWA [4];
     

    send_a_command(0x01); //Clear Screen 0x01 = 00000001
    _delay_ms(50);
    send_a_command(0x38);
    _delay_ms(50);
    send_a_command(0b00001111);
    _delay_ms(50);
    
    while(1)
    {
        COUNTA = ADCW/4;
        if(po){
            ADCSRA |=(1<<ADSC);        
            send_a_string ("Temp(C)=");
            itoa(COUNTA,SHOWA,10);
            send_a_string(SHOWA);
            send_a_string ("      ");
            send_a_command(0x80 + 0);
        }
        else {
            ADCSRA |=(1<<ADSC);
            send_a_string ("Temp(F)=");
            COUNTA = (9.5 * COUNTA) + 32;
            itoa(COUNTA,SHOWA,10);
            send_a_string(SHOWA);
            send_a_string ("      ");
            send_a_command(0x80 + 0);
        }

        ADCSRA |=(1<<ADSC);
        
    }    
}



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

