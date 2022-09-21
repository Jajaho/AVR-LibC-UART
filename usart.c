#include <avr/io.h>
#include "stddef.h"
#include <util/setbaud.h>
#include "usart.h"

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
    
    // Enable Receiver, Transmitter and Interrupt on RX complete
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    // Asynchronous mode - default

    // Set character size to 8-bit
    CLRBIT(UCSR0B, UCSZ02);
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ01);

}

void uart_echo(void) {
    // Check if data is available and the uart is ready 
    if (!BITSET(UCSR0A, RXC0)) {
        return;
    }
    if (!BITSET(UCSR0A, UDRE0)) {
        return;
    }
    UDR0 = UDR0;    // write data from the receive buffer to the transmit buffer (same I/O address)
}


uint8_t uart_getc(void) {
    while (!(UCSR0A & (1 << RXC0))) 
        ;
    return UDR0;
}

void uart_gets(char *buffer, uint8_t size) {
    uint8_t nextChar;
    uint8_t stringLen = 0;
    nextChar = uart_getc();
    while (nextChar != '\n' && stringLen < size - 1)        
    {
        *buffer++ = nextChar;
        stringLen++;
        nextChar = uart_getc();
    }
    *buffer = '\0';
}

void uart_putc(unsigned char c) {
    while (!BITSET(UCSR0A, UDRE0)) {    // uart not ready
    }
    UDR0 = c;
}

void uart_puts(char *s) {
    while(*s != '\0') {
        uart_putc(*s++);    // ++ binds closer then *, so address is iterated - not the value
    }
}