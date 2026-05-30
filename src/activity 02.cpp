#include <avr/io.h>
#include <util/delay.h>

int activity02(void)
{

    DDRC |= (1 << DDC0) |
            (1 << DDC1) |
            (1 << DDC2) |
            (1 << DDC3);


    while (1){

        PORTC = (1 << PORTC0);
        _delay_ms(1000);

        PORTC = (1 << PORTC1);
        _delay_ms(1000);

        PORTC = (1 << PORTC2);
        _delay_ms(1000);

        PORTC = (1 << PORTC3);
        _delay_ms(1000);
    }
}