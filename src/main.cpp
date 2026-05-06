#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    // Set PB5 (Arduino Uno Pin 13) as output
    DDRB |= (1 << DDB5);

    while (1)
    {
        // LED ON
        PORTB |= (1 << PORTB5);
        _delay_ms(1000);

        // LED OFF
        PORTB &= ~(1 << PORTB5);
        _delay_ms(1000);
    }

    return 0;
}