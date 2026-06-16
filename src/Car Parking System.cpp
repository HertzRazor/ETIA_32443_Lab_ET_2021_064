

#include <avr/io.h>
#include <util/delay.h>

#define CAPACITY 10

uint8_t vehicles = 0;
uint8_t previous_detect = 0;

int carParkingSystem(void)
{
   
    DDRB |= (1 << DDB0); 
    DDRB |= (1 << DDB1); 
    DDRB |= (1 << DDB2); 

    
    DDRD &= ~(1 << DDD4);
    PORTD |= (1 << PORTD4);

    
    DDRD |= (1 << DDD2);    
    DDRD &= ~(1 << DDD3);    

    while (1)
    {
        uint16_t count = 0;

        
        PORTD &= ~(1 << PORTD2);
        _delay_us(2);

        PORTD |= (1 << PORTD2);
        _delay_us(10);

        PORTD &= ~(1 << PORTD2);

        while (!(PIND & (1 << PIND3)));

        while (PIND & (1 << PIND3))
        {
            count++;
            _delay_us(1);

            if (count > 2000)
                break;
        }

        uint8_t detected = 0;

        if (count < 300)
        {
            detected = 1;
        }

        
        if (detected && !previous_detect)
        {
            if (vehicles < CAPACITY)
            {
                vehicles++;
            }
        }

        previous_detect = detected;

        
        if (!(PIND & (1 << PIND4)))
        {
            vehicles = 0;
            _delay_ms(200);
        }

        uint8_t available = CAPACITY - vehicles;

        
        PORTB &= ~((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2));

        if (available > 5)
        {
            PORTB |= (1 << PORTB0);  
        }
        else if (available > 0)
        {
            PORTB |= (1 << PORTB1);   
        }
        else
        {
            PORTB |= (1 << PORTB2);   
        }

        _delay_ms(100);
    }
}