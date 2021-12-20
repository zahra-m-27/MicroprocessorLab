#include <mega16.h>
#include <delay.h>
#include <alcd.h>
#include <stdlib.h>

char temp[32];
 

void stop(void){ 
    PORTD=0b000000;
    lcd_clear();
    lcd_puts("Stop");
} 
void moveLeftSlow(int speed){ 
    lcd_clear();           
    lcd_gotoxy(0,0);
    lcd_putsf("LEFT");
    lcd_gotoxy(0,1);
    lcd_putsf("speed: ");
    itoa(speed,temp); 
    lcd_gotoxy(7,1);
    lcd_puts(temp);
    
    DDRD.5 = 1;
    DDRD.4 = 1; 
    TCNT1 = 0;
    OCR1A = 200;
    ICR1H = 0x03;
    ICR1L = 0xE7;
    TCCR1A = 0b10100010;
    TCCR1B = 0b00010001;
                          
    PORTD=0b001000;
    delay_ms(10);
} 
void moveLeft(int speed){ 
    lcd_clear();               
    lcd_gotoxy(0,0);
    lcd_putsf("LEFT");
    lcd_gotoxy(0,1);
    lcd_putsf("speed: ");
    itoa(speed,temp);
    lcd_gotoxy(7,1);
    lcd_puts(temp);
                
    PORTD=0b001000;
    delay_ms(10); 
}
void moveLeftFast(int speed){ 
    lcd_clear();               
    lcd_gotoxy(0,0);
    lcd_putsf("LEFT");
    lcd_gotoxy(0,1);
    lcd_putsf("speed: ");
    itoa(speed,temp);
    lcd_gotoxy(7,1);
    lcd_puts(temp);
                
    PORTD=0b001001;
    delay_ms(10); 
}
void moveForward(int speed){ 
    lcd_clear();              
    lcd_gotoxy(0,0);
    lcd_putsf("FORWARD");
    lcd_gotoxy(0,1);
    lcd_putsf("speed: ");
    itoa(speed,temp);
    lcd_gotoxy(7,1);
    lcd_puts(temp);
                
    PORTD=0b000101;
    delay_ms(10);
} 
void moveRightSlow(int speed){ 
    lcd_clear();           
    lcd_gotoxy(0,0);
    lcd_putsf("RIGHT");
    lcd_gotoxy(0,1);
    lcd_putsf("speed: ");
    itoa(speed,temp); 
    lcd_gotoxy(7,1);
    lcd_puts(temp);
    
    DDRD.5 = 1;
    DDRD.4 = 1; 
    TCNT1 = 0;
    OCR1B = 200;
    ICR1H = 0x03;
    ICR1L = 0xE7;
    TCCR1A = 0b10100010;
    TCCR1B = 0b00010001;
                          
    PORTD=0b000001;
    delay_ms(10);
} 
void moveRight(int speed){ 
    lcd_clear();           
    lcd_gotoxy(0,0);
    lcd_putsf("RIGHT");
    lcd_gotoxy(0,1);
    lcd_putsf("speed: ");
    itoa(speed,temp); 
    lcd_gotoxy(7,1);
    lcd_puts(temp);
                
    PORTD=0b000001;
    delay_ms(10);
}

void moveRightFast(int speed){ 
    lcd_clear();           
    lcd_gotoxy(0,0);
    lcd_putsf("RIGHT");
    lcd_gotoxy(0,1);
    lcd_putsf("speed: ");
    itoa(speed,temp); 
    lcd_gotoxy(7,1);
    lcd_puts(temp);
                
    PORTD=0b001001;
    delay_ms(10);
}


void main(void){

    PORTA=0x00;
    DDRA=0x00;
    PORTC=0x00; 
    DDRC=0x0F;  
    PORTD=0x00; 
    DDRD=0x0F;  

    // Timer0 initialization
    TCCR0=0x00;
    TCNT0=0x00;


    // Timer1 initialization
    TCCR1A=0x00;
    TCCR1B=0x00;
    TCNT1H=0x00;
    TCNT1L=0x00;
    ICR1H=0x00;
    ICR1L=0x00;
    OCR1AH=0x00;
    OCR1AL=0x00;
    OCR1BH=0x00;
    OCR1BL=0x00;

    // Timer2 initialization
    ASSR=0x00;
    TCCR2=0x00;
    TCNT2=0x00;
    OCR2=0x00;

    // External Interupts initialization
    MCUCR=0x00;

    // Timers Interupts initialization
    TIMSK=0x00;

    // Analog Comparator initialization
    ACSR=0x80;
    SFIOR=0x00; 
    
    lcd_clear();
    lcd_init(20);
     
    while(1){   
        
        if(PORTA == 0)
            stop();
        
        if(PINA.0 == 1)
            moveLeftFast(30);
                         
        if(PINA.1 == 1)
            moveLeft(20);
            
        if(PINA.2 == 1){
            moveLeftSlow(10);
            DDRD=0x0F;
        } 
               
        if(PINA.3 == 1)
            moveForward(20);
            
        if(PINA.4 == 1){
            moveRightSlow(10);
            DDRD=0x0F; 
        }
            
        if(PINA.5 == 1)
            moveRight(20); 
            
        if(PINA.6 == 1)
            moveRightFast(30); 
            
        }; 
} 

