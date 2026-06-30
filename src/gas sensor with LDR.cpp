#include <avr/io.h>

// Initialize ADC
void ADC_Init(void)
{
    ADMUX = (1 << REFS0);                          // Use AVCC as reference voltage

    ADCSRA = (1 << ADEN) |                         // Enable ADC
             (1 << ADPS2) |
             (1 << ADPS1) |
             (1 << ADPS0);                         // Prescaler = 128
}

// Read selected ADC channel
uint16_t ADC_Read(uint8_t channel)
{
    uint8_t lowByte;
    uint8_t highByte;

    channel &= 0x07;                              // Limit channel number to 0-7

    ADMUX = (ADMUX & 0xF8) | channel;             // Select ADC channel

    ADCSRA |= (1 << ADSC);                        // Start conversion

    while (ADCSRA & (1 << ADSC));                 // Wait until conversion completes

    lowByte = ADCL;                               // Read low byte first
    highByte = ADCH;                              // Read high byte second

    return ((uint16_t)highByte << 8) | lowByte;   // Combine into 10-bit result
}

int lab6_main(void)
{
    uint16_t ldrValue;
    uint16_t potValue;
    uint16_t gasValue;

    uint16_t gasThreshold = 200;                  // Gas alert threshold

    DDRD |= (1 << PD2);                           // Set PD2 as output
    DDRD |= (1 << PD3);                           // Set PD3 as output

    ADC_Init();                                   // Initialize ADC

    while (1)
    {
        ldrValue = ADC_Read(0);                   // Read LDR on ADC0

        potValue = ADC_Read(1);                   // Read potentiometer on ADC1

        gasValue = ADC_Read(2);                   // Read gas sensor on ADC2

        if (ldrValue < potValue)                  // Check light level
        {
            PORTD |= (1 << PD2);                  // Turn lighting LED ON
        }
        else
        {
            PORTD &= ~(1 << PD2);                 // Turn lighting LED OFF
        }

        if (gasValue > gasThreshold)              // Check gas concentration
        {
            PORTD |= (1 << PD3);                  // Turn gas alert LED ON
        }
        else
        {
            PORTD &= ~(1 << PD3);                 // Turn gas alert LED OFF
        }
    }
}