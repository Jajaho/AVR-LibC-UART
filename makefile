# source file with main method
FILENAME = main
PORT = usb
DEVICE = atmega328p
PROGRAMMER = avrisp2
COMPILE = avr-gcc -Wall -Os -mmcu=${DEVICE}

default: compile upload clean

# Avr-gcc compiler settings
compile:
	${COMPILE} -c ${FILENAME}.c -o ${FILENAME}.o
	${COMPILE} -o ${FILENAME}.elf ${FILENAME}.o
# only take text and data from .elf file, would also work without this but saves space
# ihex = Intel Hexadecimal
	avr-objcopy -j .text -j .data -O ihex ${FILENAME}.elf ${FILENAME}.hex
# Get the file size
	avr-size --format=avr --mcu=${DEVICE} ${FILENAME}.elf

upload:
# -v Verbose console output for debuging
	avrdude -v -p ${DEVICE} -c ${PROGRAMMER} -P ${PORT} -U flash:w:${FILENAME}.hex:i

clean:
	rm ${FILENAME}.o
	rm ${FILENAME}.elf
	rm ${FILENAME}.hex

