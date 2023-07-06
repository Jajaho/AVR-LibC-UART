#include "defines.h"

#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>     // AVR device-specific IO definitions
#include <avr/delay.h>  // Functions for busy-wait delay loops

#include "uart.h"

char buf[40], s[40];
// stdio stream initialization by _FDEV_SETUP_RW
FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

int main(void) {
    uart_init();
    SET(DDR, BOARD_LED);
    // The default stdio streams stdin, stdout, and stderr are set up (using the same buffer) by using the existing static FILE stream objects.
    // allows to use the shorthand functions (e.g. printf() instead of fprintf())
    stdout = stdin = &uart_str;

    fprintf(stdout, "I will echo anything you type!\n");
    while (1)
    {
        if (fgets(buf, sizeof buf - 1, stdin) != NULL) {
        fprintf(stdout, "You entered: %s\n", buf);
        }
    } 
}