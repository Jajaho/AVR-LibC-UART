#include <avr/io.h>
#include "stddef.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "usart.h"

static char rxBuffer[32] = "";
static uint8_t nextFree = 0;

int main(void) {
    DDRB = (1 << PB5);
    uart_init();
    sei();

    while (1)
    {
        //blink();
        if (nextFree == 3) {
            uart_puts(&rxBuffer);
            //*rxBuffer = "";
            nextFree = 0;
        }
        _delay_us(100);
    }
    
}

void parse_cmd(char *input) {
    char *ieeePointer = strchr(input, '*');
    char *cmdPointer = strchr(input, ':');
    if (!*ieeePointer && !cmdPointer) {
        nextFree = 0;
    }
}

void rst_rxBuffer(void) {
    memset(&rxBuffer,0,strlen(&rxBuffer));
    nextFree = 0;
}

void uart_ping(void) {
    // Wait for the data register to be empty
    while (!(UCSR0A & (1<<UDRE0))) {
        blink_twice();
    }
    UDR0 = 'x';
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

ISR(USART_RX_vect) {
    rxBuffer[nextFree++] = uart_getc();
}