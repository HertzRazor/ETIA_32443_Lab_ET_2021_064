
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define PWD_LEN 4

int lockpad(void) {
    DDRD &= ~((1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5)); // Inputs
    PORTD |= (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5);   // Enable internal pull-ups
    DDRD |= (1 << PD6) | (1 << PD7);                                // LEDs

    const char pwd[PWD_LEN + 1] = "2134";
    char inpwd[PWD_LEN + 1];
    uint8_t idx = 0;
    memset(inpwd, 0, sizeof(inpwd));

    while (1) {
        char pressed = 0;

        if      ((PIND & (1 << PD2)) == 0) { pressed = '2'; }
        else if ((PIND & (1 << PD3)) == 0) { pressed = '1'; }
        else if ((PIND & (1 << PD4)) == 0) { pressed = '3'; }
        else if ((PIND & (1 << PD5)) == 0) { pressed = '4'; }

        if (pressed && idx < PWD_LEN) {
            inpwd[idx++] = pressed;
            inpwd[idx] = '\0';

            while ((PIND & (1 << PD2)) == 0 ||
                   (PIND & (1 << PD3)) == 0 ||
                   (PIND & (1 << PD4)) == 0 ||
                   (PIND & (1 << PD5)) == 0);
            _delay_ms(50);
        }

        if (idx == PWD_LEN) {
            if (strcmp(inpwd, pwd) == 0) {
                PORTD |=  (1 << PD6); // Green ON
                PORTD &= ~(1 << PD7); // Red OFF
            } else {
                PORTD &= ~(1 << PD6); // Green OFF
                PORTD |=  (1 << PD7); // Red ON
            }
            memset(inpwd, 0, sizeof(inpwd));
            idx = 0;
            _delay_ms(1000);
        }
    }
    return 0;
}