#define uint unsigned int
#define ulong unsigned long

#define PI 3.1415926535f


/*
 * ARDUINO
 *
 * Couple of useful definitions and convinience methods to make my life much
 * more simple. The `playTone(int, long)` function was stolen from some melody
 * playing tutorial on Arduino website. 
 */

#define BUZZER 11
#define LED_0 3
#define LED_1 5
#define LED_2 6
#define LED_3 9
#define LED_4 10
#define LED_COUNT 5

int ledAt(int i) {
	switch (i) {
	case 0: return LED_0;
	case 1: return LED_1;
	case 2: return LED_2;
	case 3: return LED_3;
	case 4: return LED_4;
	}
}

void playTone(int tone, long duration) {
	for (long i = 0; i < duration * 1000L; i += tone * 2) {
		digitalWrite(BUZZER, HIGH);
		delayMicroseconds(tone);
		digitalWrite(BUZZER, LOW);
		delayMicroseconds(tone);
	}
}


/*
 * KNIGHT RIDER SCANNER
 *
 * So here goes the stuff related to simulation of KITT scanner.
 */

#define SCANNER_SCAN_DURATION 350
#define SCANNER_SCAN_DELAY 1000
#define SCANNER_SCAN_ROUNDS 2
#define SCANNER_SCAN_FREQUENCY 20

#define SCANNER_TONE_MIN 60
#define SCANNER_TONE_MAX 120

typedef enum {
	TO_RIGHT,
	TO_LEFT
} scanner_mode_t;

scanner_mode_t scanner_mode;

int scanner_round;
float scanner_shift;

float scanner_phase(float x) {
	return constrain(cos(x - scanner_shift), 0.0f, 1.0f);
}

void scanner_setup() {
	scanner_mode = TO_RIGHT;
	scanner_round = 0;
	scanner_shift = 0.0f;
}

void scanner_update() {
	float step = (float)SCANNER_SCAN_FREQUENCY / (float)SCANNER_SCAN_DURATION;

	switch (scanner_mode) {
	case TO_RIGHT:
		scanner_shift += step * 2.0f * PI;
		if (scanner_shift >= PI + PI / 2.0f) {
			scanner_shift = PI + PI / 2.0f;
			scanner_mode = TO_LEFT;
		}
		break;
	case TO_LEFT:
		scanner_shift -= step * 2.0f * PI;
		if (scanner_shift <= -PI / 2.0f) {
			scanner_shift = -PI / 2.0f;
			scanner_mode = TO_RIGHT;
			scanner_delay();
		}
		break;
	}
}

void scanner_flash(int i) {
	float power = scanner_phase(((float)i / (float)(LED_COUNT - 1)) * PI);
	int value = (int)(power * 255.0f + 0.5f);
	analogWrite(ledAt(i), value);
}

void scanner_flashAll() {
	for (int i = 0; i < LED_COUNT; i++) {
		scanner_flash(i);
	}
}

void scanner_tone() {
	float tone = map(
		scanner_shift,
		-PI / 2.0f, PI + PI / 2.0f,
		SCANNER_TONE_MIN, SCANNER_TONE_MAX
	);

	playTone((int)(tone + 0.5f), SCANNER_SCAN_FREQUENCY);
}

void scanner_loop() {
	scanner_update();
	scanner_flashAll();
	scanner_tone();
}

void scanner_delay() {
	scanner_round++;
	if (scanner_round < SCANNER_SCAN_ROUNDS) {
		return;
	}

	analogWrite(BUZZER, LOW);
	for (int i = 0; i < LED_COUNT; i++) {
		analogWrite(ledAt(i), LOW);
	}
	delay(SCANNER_SCAN_DELAY);
	scanner_round = 0;
}


/*
 * MAIN
 *
 * Entry point of the application.
 */

void setup() {
	pinMode(BUZZER, OUTPUT);
	for (int i = 0; i < 5; i++) {
		pinMode(ledAt(i), OUTPUT);
	}

	scanner_setup();
}

void loop() {
	scanner_loop();
}
