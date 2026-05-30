

#include <Arduino.h>      
#include <avr/io.h>
#include <util/delay.h>

int pir(void)
{
    init();


    DDRB = (1 << DDB0) | (1 << DDB1) | (1 << DDB2);

   
    DDRD = 0x00;

  
    PORTB = 0x00;

    while (1)
    {
        
        PORTB |= (1 << PB1);
        _delay_ms(1000);

        PORTB &= ~(1 << PB1);
        _delay_ms(1000);

       
        if (PIND & (1 << PD2))
        {
            
            PORTB |= (1 << PB0);

            
            tone(10, 1000);

            _delay_ms(5000);

       
            noTone(10);
        }
        else
        {
            PORTB &= ~(1 << PB0);

            noTone(10);
        }
    }
}