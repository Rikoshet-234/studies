#include <stdio.h>
#include <string.h>

#include "color.h"
#include "accel.h"
#include "format.h"


#define uint unsigned int
#define ulong unsigned long

#define HINT_BUTTON 7

#define SHOWING_DELAY 2000

#define BLINKING_TIMES 3
#define BLINKING_DELAY 500

#define GUESSING_ACCEL_TOLERANCE 30
#define GUESSING_COLOR_TOLERANCE 60
#define GUESSING_MOTION_TOLERANCE 500
#define GUESSING_DELAY 1000

void showing_setup();
void showing_loop();

void guessing_setup();
void guessing_check();
void guessing_loop();

void blinking_setup();
void blinking_loop();

enum {
	SHOWING,
	GUESSING,
	BLINKING
} state;

bool motion;
ulong motionEndTime;

int maxX, maxY;

color_t goalColor;

void setup() {
	Serial.begin(9600);
	pinMode(HINT_BUTTON, INPUT);

	color_setup();
	accel_setup();

	showing_setup();
}

void showing_setup() {
	state = SHOWING;
	goalColor = color_rand();
	goalColor.b = 0;

	printlnf("Color to guess: (r=%d, g=%d)", goalColor.r, goalColor.g);
}

void showing_loop() {
	color_show(goalColor);
	delay(SHOWING_DELAY);
	color_hide();

	guessing_setup();
}

void guessing_setup() {
	state = GUESSING;
	motion = false;
}

void guessing_check() {
	printlnf("Guessed color: (r=%d, g=%d)", maxX, maxY);

	if (abs(maxX - goalColor.r) < GUESSING_COLOR_TOLERANCE &&
	    abs(maxY - goalColor.g) < GUESSING_COLOR_TOLERANCE)
	{
		printlnf("Guessed correctly!");
		blinking_setup();
	} else {
		printlnf("Your guess was incorrect :<");
		color_show(color_new(maxX, maxY, 0));
		delay(GUESSING_DELAY);
		color_hide();
	}
}

void guessing_loop() {
	int x = accel_readX();
	int y = accel_readY();
	ulong currentTime = millis();
	// printlnf("%d %d", x, y);

	if (digitalRead(HINT_BUTTON) == HIGH) {
		color_show(goalColor);
	} else {
		color_hide();
	}

	if (!motion && (x >= GUESSING_ACCEL_TOLERANCE ||
	                y >= GUESSING_ACCEL_TOLERANCE))
	{
		/*
		 * Acceleration is greater than tolerance, therefore the motion has
		 * started. Therefore we must initialize guessed color.
		 */
		printlnf("Motion started!");

		motion = true;
		maxX = 0;
		maxY = 0;
	} else if (motion && (x < GUESSING_ACCEL_TOLERANCE &&
	                      y < GUESSING_ACCEL_TOLERANCE))
	{
		/*
		 * Acceleration is lower than tolerance and we were in motion state,
		 * therefore motion has ended. Check whether user guessed the color
		 * right and go to the blinking state if so. Otherwise, show what he
		 * guessed and start guessing proces again.
		 */
		if (currentTime - motionEndTime >= GUESSING_MOTION_TOLERANCE) {
			printlnf("Motion ends!");
			printlnf("Border values: (%d, %d)", maxX, maxY);

			motion = false;
			guessing_check();
		}
	}

	if (motion && (x >= GUESSING_ACCEL_TOLERANCE ||
	               y >= GUESSING_ACCEL_TOLERANCE))
	{
		/*
		 * We are in the motion state, so we take maximum of current
		 * acceleration and guess so far as the new guess color value.
		 */
		motionEndTime = currentTime;
		maxX = max(maxX, x);
		maxY = max(maxY, y);
	}
}

void blinking_setup() {
	state = BLINKING;
}

void blinking_loop() {
	for (int i = 0; i < BLINKING_TIMES; i++) {
		color_show(goalColor);
		delay(BLINKING_DELAY);
		color_hide();
		delay(BLINKING_DELAY);
	}

	showing_setup();
}

void loop() {
	accel_loop();

	switch (state) {
	case SHOWING:
		showing_loop();
		break;
	case GUESSING:
		guessing_loop();
		break;
	case BLINKING:
		blinking_loop();
		break;
	}

	delay(10);
}
