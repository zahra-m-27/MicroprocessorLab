#include <avr/io.h>    
#include <avr/delay.h>
#include <avr/interrupt.h>


#define a1 (1<<PD4)
#define a2 (1<<PD5)
#define a3 (1<<PD6)
#define a4  (1<<PD7)
#define Switch (1<<PC2)

 
int i=0; 
volatile uint8_t count =0;
unsigned char step[4]= {0x90, 0x50, 0x60, 0xa0};

int main(void){

	DDRD |= 0xf0;

    DDRC |= 0x00;
	PORTC |= Switch;

    TCNT0 = 0x00;
    TCCR0 |= (1 << CS02) | (1 << CS00);
    TIMSK |= (1 << TOIE0);
    TIFR |= (1 << TOV0);
    sei();



	while(1){

		if(count>=3 & !(PINC & Switch)){
            TCNT0 = 0;
            while (TCNT0 <= 113);
            
            PORTD = step[i];
            TCNT0 = 0;
            count = 0; 
            i++;
            if(i == 4){
                i = 0;
            }
             
	    }
    }
    return(0);

}

  ISR(TIMER0_OVF_vect){
      count++;
   }