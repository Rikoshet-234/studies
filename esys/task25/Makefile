PORT=/dev/ttyACM0
CFLAGS=-std=c99 -Wall -Wextra -pedantic

build: task25.hex

install: build
	avrdude -v -c arduino -p m328p -b 115200 -P $(PORT) -U flash:w:task25.hex

task25.elf: task25.c
	avr-gcc $(CFLAGS) -Os -DF_CPU=16000000UL -mmcu=atmega328p -o $@ $^

task25.hex: task25.elf
	avr-objcopy -O ihex -R .eeprom task25.elf task25.hex

clean:
	rm -rf *.o *.elf *.hex
