#include <avr/io.h>
#include <stdint.h>
#include "stddef.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "usart.h"

static char rxBuffer[32] = "";
static uint8_t nextFree = 0;

// strcasestr()
// char * strsep(char **stringp, const char *delim);
void parse_cmd(char *input) {
    // Pointer to array, will be overriden by strsep() with pointer to first char after a found delimeter, or NULL ptr. if the end of the string was reached
    char *start = rxBuffer;          
    char **header[MAX_DEL_COUNT];    // Array of pointers to the first char after a found delimeter
    //uint8_t delCount = 0;           // Number of delimeters found in the input string
    uint8_t newFirstIndex = 0;      // First index that the buffer will have at the end of this method

    for (uint8_t i = 0; strsep(&start, ":") && i < MAX_DEL_COUNT; i++) {
        header[i] = start;
        //delCount++;
    }
    /*
    for (uint8_t i = 0; i < delCount; i++) {
        uart_puts(header[i]);
    }
    */
    
    // match mnemonic keywords, strcasecmp returns 0 on match
    if (header[0]) {
        if (!strcasecmp(header[0], "meas") || !strcasecmp(header[0], "measure")) {
            if (header[1]) { 
                if (!strcasecmp(header[1], "ch1") || !strcasecmp(header[1], "channel1")) {
                    if (header[2]) { 
                        if (!strcasecmp(header[2], "esr")) {
                            uart_puts("Measuring ESR on CH1.");
                            // advance ringbuffer to next delimeter
                        }
                    }
                }
            }
        }
    }
    uart_puts(rxBuffer);
}

void flush_rxBuffer(void) {
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

int main(void) {
    DDRB = (1 << PB5);
    uart_init();
    sei();

    while (1)
    {
        blink();
        
        parse_cmd(rxBuffer);

        _delay_us(100);
    }
    
}

ISR(USART_RX_vect) {
    rxBuffer[nextFree++] = uart_getc();
}