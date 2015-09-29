/* Pin configuration. */
#define DIODE 2
#define BUZZER 3

/* Useful constants. */
#define ON true
#define OFF false

/* Program settings. */
#define UNIT_DURATION 200
#define DOT_DURATION UNIT_DURATION
#define DASH_DURATION (3 * UNIT_DURATION)
#define PITCH_FREQUENCY 128
#define SERIAL_RATE 9600

static const char *MORSE_CODES[] = {
	/* A */ ".--",
	/* B */ "-...",
	/* C */ "-.-.",
	/* D */ "-..",
	/* E */ ".",
	/* F */ "..-.",
	/* G */ "--.",
	/* H */ "....",
	/* I */ "..",
	/* J */ ".---",
	/* K */ "-.-",
	/* L */ ".-..",
	/* M */ "--",
	/* N */ "-.",
	/* O */ "---",
	/* P */ ".--.",
	/* Q */ "--.-",
	/* R */ ".-.",
	/* S */ "...",
	/* T */ "-",
	/* U */ "..-",
	/* V */ "...-",
	/* W */ ".--",
	/* X */ "-..-",
	/* Y */ "-.--",
	/* Z */ "--.."
};

void signal(bool enabled) {
	if (enabled) {
		digitalWrite(DIODE, HIGH);
		analogWrite(BUZZER, PITCH_FREQUENCY);
	} else {
		digitalWrite(DIODE, LOW);
		analogWrite(BUZZER, 0);
	}
}

void morseCharacter(char c) {
	signal(ON);
	switch (c) {
	case '.':
		Serial.println(".");
		delay(DOT_DURATION);
		break;
	case '-':
		Serial.println("-");
		delay(DASH_DURATION);
		break;
	default:
		break;
	}

	signal(OFF);
	delay(UNIT_DURATION);
}

void morseString(const char *str) {
	while (*str != '\0') {
		morseCharacter(*str);
		str++;
	}
}

void playCharacter(char c) {
	if ('a' <= c && c <= 'z') {
		morseString(MORSE_CODES[c - 'a']);
	} else if ('A' <= c && c <= 'Z') {
		morseString(MORSE_CODES[c - 'A']);
	}
}

void setup() {
	pinMode(DIODE, OUTPUT);
	pinMode(BUZZER, OUTPUT);

	Serial.begin(SERIAL_RATE);
}

void loop() {
	while (Serial.available()) {
		playCharacter(Serial.read());
	}

	signal(OFF);
}
