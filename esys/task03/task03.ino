#include <math.h>

/* Pin configuration. */
#define DIODE_R 9
#define DIODE_G 10
#define DIODE_B 11

/* World constants. */
#define PI (4.0 * atan(1.0))
#define ANALOG_MIN 0
#define ANALOG_MAX 255

/* Program settings. */
#define BREATH_TIME 4000

/* Color manipulation utilities. */
struct color_t {
	int r, g, b;
};

struct color_t color_random() {
	color_t result;
	result.r = random(ANALOG_MIN, ANALOG_MAX);
	result.g = random(ANALOG_MIN, ANALOG_MAX);
	result.b = random(ANALOG_MIN, ANALOG_MAX);
	return result;
}

struct color_t color_scale(color_t color, float factor) {
	color_t result;
	result.r = (int)(factor * (float)color.r + 0.5f);
	result.g = (int)(factor * (float)color.g + 0.5f);
	result.b = (int)(factor * (float)color.b + 0.5f);
	return result;
}

/* Main program logic. */
unsigned long currentTime, breathStartTime;
color_t targetColor;

void reset() {
	targetColor = color_random();
	breathStartTime = currentTime;
}

void setup() {
	pinMode(DIODE_R, OUTPUT);
	pinMode(DIODE_G, OUTPUT);
	pinMode(DIODE_B, OUTPUT);
	
	randomSeed(analogRead(0));

	reset();
}

void loop() {
	currentTime = millis();
	unsigned long duration = currentTime - breathStartTime;

	float progress = (float)duration / (float)BREATH_TIME;
	float power = constrain(sin(PI * progress), 0.0, 1.0);
	color_t diode = color_scale(targetColor, power);

	analogWrite(DIODE_R, ANALOG_MAX - diode.r);
	analogWrite(DIODE_G, ANALOG_MAX - diode.g);
	analogWrite(DIODE_B, ANALOG_MAX - diode.b);

	if (duration >= BREATH_TIME) {
		reset();
	}

	delay(10);
}
