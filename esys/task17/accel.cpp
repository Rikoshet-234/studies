#include "accel.h"

#include "Arduino.h"


#define ACCEL_AXIS_X A0
#define ACCEL_AXIS_Y A1

static int accel_prevValueX;
static int accel_prevValueY;
static int accel_currValueX;
static int accel_currValueY;

void accel_setup() {
	pinMode(ACCEL_AXIS_X, INPUT);
	pinMode(ACCEL_AXIS_Y, INPUT);
	accel_prevValueX = accel_currValueX = analogRead(ACCEL_AXIS_X);
	accel_prevValueY = accel_currValueY = analogRead(ACCEL_AXIS_Y);
}

void accel_loop() {
	accel_prevValueX = accel_currValueX;
	accel_currValueX = analogRead(ACCEL_AXIS_X);

	accel_prevValueY = accel_currValueY;
	accel_currValueY = analogRead(ACCEL_AXIS_Y);
}

int accel_readX() {
	return abs(accel_currValueX - accel_prevValueX);
}

int accel_readY() {
	return abs(accel_currValueY - accel_prevValueY);	
}
