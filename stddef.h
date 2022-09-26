#ifndef STDDEF_H_
#define STDDEF_H_

#define BAUD 9600


#define MIN_DEL_COUNT 3
#define MAX_DEL_COUNT 4
// Pins & Ports //
#define MUX_PORT DDRD;
#define FIRST_ADDR_PIN PD2;

#define BOARD_LED PB5

// MAKROS //

#define SETBIT(port, pin) (port |= (1 << pin))
#define CLRBIT(port, pin) (port &= ~(1 << pin))
#define BITSET(port, pin) (port&(1 << pin))

#define LED_ON() SETBIT(PORTB, BOARD_LED)
#define LED_OFF() CLRBIT(PORTB, BOARD_LED)

#endif /* STDDEF_H_ */