cmake_minimum_required(VERSION 3.9)
set(CMAKE_SYSTEM_NAME Generic)
project(avr_gcc_uart)

set(MCU "atmega328p")
set(PROGRAMMER "avrisp2")
set(PORT "usb")

# --------- Toolchain Options --------- #
set(STRIP avr-strip)
set(OBJCOPY avr-objcopy)
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)
set(SIZE avr-size) # 2.19 ist the last version that support avr format and --mcu option, without them no usage can be displayed


# --------- Compiler Flags --------- #
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

# --------- Sources --------- #
set(SOURCES main.c usart.c)

# --------- Targets --------- #
add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")

add_custom_target(strip ALL ${STRIP} "${PROJECT_NAME}.elf" DEPENDS ${PROJECT_NAME})
add_custom_target(bin ALL ${OBJCOPY} -R .eeprom -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.hex" DEPENDS strip)
add_custom_target(size ALL ${SIZE} -C --mcu=${MCU} ${PROJECT_NAME}.elf DEPENDS bin)
add_custom_target(upload ALL avrdude -p ${MCU} -c ${PROGRAMMER} -P ${PORT} -U flash:w:${PROJECT_NAME}.hex:i DEPENDS bin)
add_custom_target(com picocom /dev/ttyUSB0 -b 9600)