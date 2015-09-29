/*
 * The author of this is Michael Smith and the original version of code can be
 * found here:
 * 
 *     http://playground.arduino.cc/Code/PCMAudio
 */
  
#include <stdint.h>
#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#define BUZZER 11
#define SAMPLE_RATE 8000

const prog_uint8_t *sampleData;
unsigned int sampleCount;

volatile uint16_t sample;
uint8_t lastSample;

float playbackSpeed;

void pcmp_init(const prog_uint8_t *data, uint16_t count) {
	pinMode(BUZZER, OUTPUT);

	sampleData = data;
	sampleCount = count;

	playbackSpeed = 1.0f;
}

void pcmp_setSpeed(float speed) {
	playbackSpeed = speed;
}

void pcmp_stop() {
	TIMSK1 &= ~_BV(OCIE1A);
	TCCR1B &= ~_BV(CS10);
	TCCR2B &= ~_BV(CS10);

	digitalWrite(BUZZER, LOW);
}

ISR(TIMER1_COMPA_vect) {
	if (sample >= sampleCount) {
		if (sample >= sampleCount + lastSample) {
			pcmp_stop();
		} else {
			OCR2A = sampleCount + lastSample - sample;
		}
	} else {
		OCR2A = pgm_read_byte(sampleData + sample);
	}

	sample++;
}

void pcmp_start() {
	ASSR &= ~(_BV(EXCLK) | _BV(AS2));
	TCCR2A |= _BV(WGM21) | _BV(WGM20);
	TCCR2B &= ~_BV(WGM22);

		TCCR2A = (TCCR2A | _BV(COM2A1)) & ~_BV(COM2A0);
		TCCR2A &= ~(_BV(COM2B1) | _BV(COM2B0));
		TCCR2B = (TCCR2B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);

		OCR2A = pgm_read_byte(sampleData + 0);

	cli();

	TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
	TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));

	TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
	OCR1A = F_CPU / ((int)(SAMPLE_RATE * playbackSpeed + 0.5f));

	TIMSK1 |= _BV(OCIE1A);

	lastSample = pgm_read_byte(sampleData + sampleCount - 1);
	sample = 0;
	sei();
}
