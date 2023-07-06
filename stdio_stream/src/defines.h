#ifndef DEFINES_H_
#define DEFINES_H_


#define __AVR_ATmega328P__

// ----- CPU Clock Speed & BAUD ----- //
// required for util/setbaud.h
#define F_CPU 16000000UL
#define BAUD 9600

// ----- Pin Definitions ----- //

#define BOARD_LED B, 5

// AVR LIBC MAKROS //
// See: https://www.nongnu.org/avr-libc/examples/stdiodemo/hd44780.c

#define GLUE(a, b) a##b //Conactenates two tokens

/* single-bit macros, used for control bits */
#define SET_(what, p, m) GLUE(what, p) |= (1 << (m))
#define CLR_(what, p, m) GLUE(what, p) &= ~(1 << (m))
#define GET_(/* PIN, */ p, m) GLUE(PIN, p) & (1 << (m))
#define SET(what, x) SET_(what, x)
#define CLR(what, x) CLR_(what, x)
#define GET(/* PIN, */ x) GET_(x)

/* multy-bit macros, used to read or write entire registers */

/// ASSIGN(PORT, FIRST_BIT, DATA) The data bits have to be in consequtive ascending order
#define ASSIGN_(what, p, m, v) GLUE(what, p) = (GLUE(what, p) & \
						~((1 << (m)) | (1 << ((m) + 1)) | \
						  (1 << ((m) + 2)) | (1 << ((m) + 3)))) | \
					        ((v) << (m)) 
#define READ_(what, p, m) (GLUE(what, p) & ((1 << (m)) | (1 << ((m) + 1)) | \
					    (1 << ((m) + 2)) | (1 << ((m) + 3)))) >> (m)
#define ASSIGN(what, x, v) ASSIGN_(what, x, v)
#define READ(what, x) READ_(what, x)


#endif /* DEFINES_H_ */