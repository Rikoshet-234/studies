#include "color.h"

#include "Arduino.h"

void color_setup() {
	pinMode(A0, INPUT);
	srand(analogRead(A0));

	pinMode(LED_R, OUTPUT);
	pinMode(LED_G, OUTPUT);
	pinMode(LED_B, OUTPUT);
}

color_t color_new(unsigned char r, unsigned char g, unsigned char b) {
	color_t result;
	result.r = r;
	result.g = g;
	result.b = b;
	return result;
}

color_t color_rand() {
	return color_new(rand() % 0xFF, rand() % 0xFF, rand() % 0xFF);
}

void color_show(color_t color) {
	analogWrite(LED_R, 0xFF - color.r);
	analogWrite(LED_G, 0xFF - color.g);
	analogWrite(LED_B, 0xFF - color.b);
}

void color_hide() {
	analogWrite(LED_R, 0xFF);
	analogWrite(LED_G, 0xFF);
	analogWrite(LED_B, 0xFF);
}
