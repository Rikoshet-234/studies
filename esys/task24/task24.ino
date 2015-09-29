#include "divmod.h"

void setup() {
	Serial.begin(9600);
}

void loop() {
	if (Serial.available() > 0) {
		uint16_t dend = Serial.parseInt();
		uint16_t dsor = Serial.parseInt();

		uint16_t quot, rem;
		divmod16(dend, dsor, &quot, &rem);

		Serial.println(quot);
		Serial.println(rem);
	}
}
