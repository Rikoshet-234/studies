/* Pin configuration. */
#define RESET_BUTTON 2
#define DIODE 6
#define BUZZER 11
#define MAGNETOMETER A3

/* World constants. */
#define DEFAULT_MAGNETIC_FIELD 510

/* Program settings. */
#define BUZZ_DURATION 200
#define BUZZER_SEARCH_FREQUENCY 160
#define BUZZER_FOUND_FREQUENCY 320

#define SEARCH_RADIUS 30

inline int buzzerDelay(int delta) {
	return (delta + 2 * SEARCH_RADIUS) * delta;
}

/* Main program logic. */
boolean found;
int value;

void reset() {
	found = false;

#ifdef RANODM_MODE
	value = random(0, SEARCH_RADIUS);
#else
	value = SEARCH_RADIUS;
#endif
}

void actionFound() {
	tone(BUZZER, BUZZER_FOUND_FREQUENCY, BUZZ_DURATION);
	digitalWrite(DIODE, HIGH);

	if (digitalRead(RESET_BUTTON) == HIGH) {
		reset();
	}
}

void actionSearch() {
	/*
	 * The voltage on the pin connected to the magnetometer is relative to
	 * default magnetic field voltage. Therefore, to obtain strenght of
	 * magnetic field we calculate difference between the default value and
	 * measured value.
	 */
	int strength = abs(DEFAULT_MAGNETIC_FIELD - analogRead(MAGNETOMETER));
	int delta = abs(value - strength);

	/*
	 * The code below allows to control interval between beeps. It is preferred
	 * over the delay function because does not hang the whole program and
	 * allows for immidate response to the changing magnetic field strength.
	 */
	static int buzzStartTime = 0;
	int currentTime = millis();
	if (currentTime - buzzStartTime >= buzzerDelay(delta)) {
		tone(BUZZER, BUZZER_SEARCH_FREQUENCY, BUZZ_DURATION);
		buzzStartTime = currentTime;		
	}

	digitalWrite(DIODE, LOW);

	if (delta <= 1) {
		found = true;
	}
}

void setup() {
	randomSeed(analogRead(A0));

	pinMode(RESET_BUTTON, INPUT);
	pinMode(DIODE, OUTPUT);
	pinMode(BUZZER, OUTPUT);
	pinMode(MAGNETOMETER, INPUT);

	reset();
}

void loop() {
	if (found) {
		actionFound();
	} else {
		actionSearch();
	}

	delay(100);
}
