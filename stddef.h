#ifndef STDDEF_H_
#define STDDEF_H_


#define __AVR_ATmega328P__
#define F_CPU 16000000UL

#define BAUD 9600

// Pins //

#define BOARD_LED PB5

// MAKROS //

#define SETBIT(port, pin) (port |= (1 << pin))
#define CLRBIT(port, pin) (port &= ~(1 << pin))
#define BITSET(port, pin) (port&(1 << pin))

#define LED_ON() SETBIT(PORTB, BOARD_LED)
#define LED_OFF() CLRBIT(PORTB, BOARD_LED)

#endif /* STDDEF_H_ */