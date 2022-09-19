#include "stddef.h"
#include <avr/io.h>
#include <util/setbaud.h>
#include <util/delay.h>
#include <avr/interrupt.h>



int main(void) {
    DDRB = (1 << PB5);
    while (1)
    {
        LED_ON();
        _delay_ms(1000);
        LED_OFF();
        _delay_ms(1000);
        sei();
    }
    
}

void uart_init(void) {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    #if USE_2X
        UCSR0A |= (1 << U2X0);
    #else 
        UCSR0A &= ~(1 << U2X0);
    #endif
}

