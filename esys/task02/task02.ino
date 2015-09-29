/* Pin configuration. */
#define BUTTON_MIN 2
#define BUTTON_MAX 3
#define DIODE 6
#define LIGHNTESS A5

/* World constants. */
#define ANALOG_MIN 0
#define ANALOG_MAX 255

/* Main program logic. */
int lightMin;
int lightMax;

int dimm(int lightness) {
	int clamped = constrain(lightness, lightMin, lightMax);
	return map(clamped, lightMin, lightMax, ANALOG_MIN, ANALOG_MAX);
}

void setup() {
	pinMode(BUTTON_MIN, INPUT);
	pinMode(BUTTON_MAX, INPUT);
	pinMode(LIGHNTESS, INPUT);

	lightMin = ANALOG_MIN;
	lightMax = ANALOG_MAX;
}

void loop() {
	int lightness = analogRead(LIGHNTESS);

	boolean minPressed = digitalRead(BUTTON_MIN) == HIGH;
	if (minPressed) {
		lightMin = lightness;
	}
	boolean maxPressed = digitalRead(BUTTON_MAX) == HIGH;
	if (maxPressed) {
		lightMax = lightness;
	}

	analogWrite(DIODE, dimm(lightness));
}
