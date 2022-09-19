#include "stddef.h"
#include <avr/io.h>
#include <util/setbaud.h>
#include <util/delay.h>



int main(void) {
    DDRB = (1 << PB5);

    uart_init();

    char string[9];

    while (1)
    {
        blink();
        uart_gets(string);
        blink();
        uart_puts(string);
        blink();
    }
    
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
    // Asynchronous mode - default

    // Set character size to 8-bit
    CLRBIT(UCSR0B, UCSZ02);
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ01);

}

void uart_ping(void) {
    // Wait for the data register to be empty
    while (!(UCSR0A & (1<<UDRE0))) {
        blink_twice();
    }
    UDR0 = 'x';
}

void uart_echo(void) {
    // Check if data is available and the uart is ready 
    if (!BITSET(UCSR0A, RXC0)) {
        blink_twice();
        return;
    }
    if (!BITSET(UCSR0A, UDRE0)) {
        blink_thrice();
        return;
    }
    UDR0 = UDR0;    // write data from the receive buffer to the transmit buffer (same I/O address)
}

uint8_t uart_getc(void) {
    if (!BITSET(UCSR0A, UDRE0)) {
        blink_thrice();
        return;
    }
    return UDR0;
}

void uart_gets(char* buffer) {
    uint8_t nextChar = uart_getc();
    while (nextChar != '\n')
    {
        *buffer++ = nextChar;
        nextChar = uart_getc();
    }
    *buffer = '\0';     // add c string terminator
}

void uart_putc(unsigned char c) {
    while (!BITSET(UCSR0A, UDRE0)) {
        blink_twice();
    }
    UDR0 = c;
}

void uart_puts(char *s) {
    while(*s) {     // while s* != '\0'
        uart_putc(*s++);
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
