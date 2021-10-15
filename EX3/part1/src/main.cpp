#include <avr/io.h>
#include <avr/delay.h>


int main()
{
    DDRD = 0xFF;
	TCNT1 = 0;
 	TCCR1B|=(1<<CS11);
 	TCCR1B|=(1<<CS10);

    int i=0;


    while(1){
        if(TCNT1>=65535)
		{
            switch(i % 10) {

                case 0x00:
                    PORTD = 0b01000000;
                    break;

                case 0x01:
                    PORTD = 0b01111001;
                    break;                    

                case 0x02:
                    PORTD = 0b01111001;
                    break;

                case 0x03:
                    PORTD = 0b00110000;
                    break;

                case 0x04:
                    PORTD = 0b00011001;
                    break;

                case 0x05:
                    PORTD = 0b00010010;
                    break;

                case 0x06:
                    PORTD = 0b00000010;
                    break;

                case 0x07:
                    PORTD = 0b01111000;
                    break;

                case 0x08:
                    PORTD = 0b00000000;
                    break;

                case 0x09:
                    PORTD = 0b00010000;
                    break;

                default:
                    return 0;
        }
            i++;
            TCNT1 =0;
        }

    }
}

