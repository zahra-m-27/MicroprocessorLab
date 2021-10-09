
#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/delay.h>
int main()
{
    DDRA = 1;
    PORTA = 0x01; 
    while (1){
    _delay_ms(500);
    PORTA = ~PORTA;
    }
    return 0;
}