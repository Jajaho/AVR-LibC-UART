#ifndef USART
#define USART

void uart_init(void);
void uart_echo(void);
void uart_ping(void);
uint8_t uart_getc(void);
void uart_gets(char *buffer, uint8_t size);
void uart_putc(unsigned char c);
void uart_puts(char *s);

#endif