#include "format.h"

#include <stdio.h>
#include <stdarg.h>
#include "Arduino.h"


#define FORMAT_BUFFER_SIZE 256

static char format_buffer[FORMAT_BUFFER_SIZE];

void printlnf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	vsnprintf(format_buffer, FORMAT_BUFFER_SIZE, format, args);
	va_end(args);

	Serial.println(format_buffer);
	Serial.flush();
}
