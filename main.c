#include "stddef.h"
#include <avr/io.h>
#include <util/setbaud.h>
#include <util/delay.h>



int main(void) {
    DDRB = (1 << PB5);

    uart_init();

    while (1)
    {
        uart_ping();
        blink();
    }
    
}

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

void uart_init(void) {
    // Writing the baudrate calculated by setbaud.h to the "USART Baud Rate Register"
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    // USE_2X is set by setbaud.h if the prescaler is required
    #if USE_2X
        UCSR0A |= (1 << U2X0);
    #else 
        UCSR0A &= ~(1 << U2X0);
    #endif
    
    // Enable Receiver and Transmitter
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    // asynchronous mode - default
    
    // Set character size to 8-bit
    CLRBIT(UCSR0B, UCSZ02);
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ01);

}

void uart_ping(void) {
    while (!(UCSR0A & (1<<UDRE0))) {
        blink_twice();
    }
    UDR0 = 'x';
}

