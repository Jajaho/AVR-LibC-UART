cmake_minimum_required(VERSION 3.9)
set(CMAKE_SYSTEM_NAME Generic)
project(avr_gcc_uart)

set(MCU "atmega328p")
set(PROGRAMMER "avrisp2")
set(PORT "usb")

# Compiler options
set(AVRSTRIP avr-strip)
set(OBJCOPY avr-objcopy)
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)

set(CDEBUG "-g")
set(CWARN "-Wall -Wextra -Wstrict-prototypes")
#set(CTUNING "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums")
#set(CINCS "-Iinclude")
set(COPT "-Os")
set(CDEFS "-DF_CPU=16000000UL")
set(CFLAGS "-mmcu=${MCU} ${CDEBUG} ${CDEFS} ${CINCS} ${COPT} ${CWARN}")
set(CXXFLAGS "${CMCU} ${CDEFS} ${CINCS} ${COPT}")
set(CMAKE_C_FLAGS  ${CFLAGS})
set(CMAKE_CXX_FLAGS ${CXXFLAGS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD  11)

set(SOURCES main.c usart.c)

add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")

add_custom_target(strip ALL ${AVRSTRIP} "${PROJECT_NAME}.elf" DEPENDS ${PROJECT_NAME})
add_custom_target(compile ALL ${OBJCOPY} -R .eeprom -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.hex" DEPENDS strip)
add_custom_target(upload ALL avrdude -v -p ${MCU} -c ${PROGRAMMER} -P ${PORT} -U flash:w:${PROJECT_NAME}.hex:i DEPENDS compile)
add_custom_target(com picocom /dev/ttyUSB0 -b 9600)