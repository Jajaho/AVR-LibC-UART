#include <avr/io.h>
#include "stddef.h"
#include <util/delay.h>

void blink(void) {
    _delay_ms(500);
    LED_ON();
    _delay_ms(200);
    LED_OFF();
    _delay_ms(500);
}

void blink_twice(void) {
    _delay_ms(500);
    LED_ON();
    _delay_ms(200);
    LED_OFF();
    _delay_ms(200);
    LED_ON();
    _delay_ms(200);
    LED_OFF();
    _delay_ms(500);
}

void blink_thrice(void) {
    _delay_ms(500);
    LED_ON();
    _delay_ms(200);
    LED_OFF();
    _delay_ms(200);
    LED_ON();
    _delay_ms(200);
    LED_OFF();
    _delay_ms(200);
    LED_ON();
    _delay_ms(200);
    LED_OFF();
    _delay_ms(500);
}