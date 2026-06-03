
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t emergencyFlag = 0;
volatile uint8_t pedestrianFlag = 0;
volatile uint8_t maintenanceFlag = 0;

// Emergency Button - INT0
ISR(INT0_vect)
{
    emergencyFlag = 1;
}

// Pedestrian Button - INT1
ISR(INT1_vect)
{
    pedestrianFlag = 1;
}

// Maintenance Button - Pin Change Interrupt
ISR(PCINT2_vect)
{
    if (!(PIND & (1 << PD4)))
    {
        maintenanceFlag ^= 1;
        _delay_ms(50);
    }
}

void roadRed()
{
    PORTB = (1 << PB0);
    PORTD |= (1 << PD6);
    PORTD &= ~(1 << PD7);
}

void roadGreen()
{
    PORTB = (1 << PB2);
    PORTD |= (1 << PD7);
    PORTD &= ~(1 << PD6);
}

void yellowOnly()
{
    PORTB = (1 << PB1);
}

int traffic_light_system(void)
{
    // LEDs
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);
    DDRD |= (1 << PD6) | (1 << PD7);

    // Buttons
    DDRD &= ~((1 << PD2) | (1 << PD3) | (1 << PD4));

    // Internal Pullups
    PORTD |= (1 << PD2) | (1 << PD3) | (1 << PD4);

    // INT0 Falling Edge
    EICRA |= (1 << ISC01);
    EIMSK |= (1 << INT0);

    // INT1 Falling Edge
    EICRA |= (1 << ISC11);
    EIMSK |= (1 << INT1);

    // Pin Change Interrupt for D4
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT20);

    sei();

    while (1)
    {
        // Maintenance Mode
        if (maintenanceFlag)
        {
            PORTB &= ~((1 << PB0) | (1 << PB2));
            PORTD &= ~((1 << PD6) | (1 << PD7));

            while (maintenanceFlag)
            {
                PORTB ^= (1 << PB1);
                _delay_ms(500);
            }

            PORTB &= ~(1 << PB1);
        }

        // Emergency Mode
        if (emergencyFlag)
        {
            roadGreen();
            _delay_ms(10000);

            emergencyFlag = 0;
        }

        // Pedestrian Mode
        else if (pedestrianFlag)
        {
            yellowOnly();
            _delay_ms(5000);

            roadRed();
            _delay_ms(10000);

            pedestrianFlag = 0;
        }

        // Normal Traffic Operation
        else
        {
            roadGreen();

            for (uint8_t i = 0; i < 50; i++)
            {
                _delay_ms(100);

                if (emergencyFlag || pedestrianFlag || maintenanceFlag)
                    break;
            }

            roadRed();

            for (uint8_t i = 0; i < 50; i++)
            {
                _delay_ms(100);

                if (emergencyFlag || pedestrianFlag || maintenanceFlag)
                    break;
            }
        }
    }
}
