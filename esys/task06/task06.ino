/* Pin configuration. */
#define DIODE 2
#define BUZZER 3
#define BUTTON 4

/* Useful constants. */
#define ON true
#define OFF false

/* Program settings. */
#define UNIT_DURATION 300
#define DOT_DURATION UNIT_DURATION
#define DASH_DURATION (3 * UNIT_DURATION)
#define LETTER_SPACE_DURATION (3 * UNIT_DURATION)
#define WORD_SPACE_DURATION (7 * UNIT_DURATION)

#define PITCH_FREQUENCY 128
#define SERIAL_RATE 9600

#define CODE_BUFFER_CAPACITY 8
#define WORD_BUFFER_CAPACITY 64

static const char *MORSE_CODES[] = {
	/* A */ ".-",
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

/* Character buffer (aka string) utlility class. */
struct str_t {
	char *buffer;
	unsigned int size;
};

inline void str_init(struct str_t *str, char *buffer) {
	str->buffer = buffer;
	str_clear(str);
}

inline void str_put(str_t *str, char c) {
	str->buffer[str->size++] = c;
	str->buffer[str->size] = '\0';
}

inline void str_clear(str_t *str) {
	str->size = 0;
	str->buffer[str->size] = '\0';
}

inline bool str_isEmpty(str_t *str) {
	return str->size == 0;
}

/* Main program logic. */
static char codeBuffer[CODE_BUFFER_CAPACITY];
str_t codeStr;

static char wordBuffer[WORD_BUFFER_CAPACITY];
str_t wordStr;

bool isPressed, wasPressed;
unsigned long actionStartTime;

char recognizeCode() {
	for (char c = 'A'; c <= 'Z'; c++) {
		if (strcmp(MORSE_CODES[c - 'A'], codeBuffer) == 0) {
			return c;
		}
	}

	return '?';
}

void onButtonPressed() {
	unsigned long currentTime = millis();
	unsigned long idleDuration = currentTime - actionStartTime;

	if (idleDuration >= LETTER_SPACE_DURATION && !str_isEmpty(&codeStr)) {
		str_put(&wordStr, recognizeCode());
		str_clear(&codeStr);
	}

	actionStartTime = currentTime;
}

void onButtonReleased() {
	unsigned long currentTime = millis();
	unsigned long pressDuration = currentTime - actionStartTime;

	if (pressDuration <= DOT_DURATION) {
		str_put(&codeStr, '.');
	} else if (pressDuration <= DASH_DURATION) {
		str_put(&codeStr, '-');
	} else {
		str_clear(&codeStr);
		str_clear(&wordStr);
	}

	actionStartTime = currentTime;
}

void onButtonIdle() {
	unsigned long currentTime = millis();
	unsigned long idleDuration = currentTime - actionStartTime;

	if (idleDuration >= WORD_SPACE_DURATION) {
		if (!str_isEmpty(&codeStr)) {
			str_put(&wordStr, recognizeCode());
			str_clear(&codeStr);
		}

		if (!str_isEmpty(&wordStr)) {
			Serial.println(wordStr.buffer);
			str_clear(&wordStr);
		}
	}
}

void signal(bool enabled) {
	if (enabled) {
		digitalWrite(DIODE, HIGH);
		analogWrite(BUZZER, PITCH_FREQUENCY);
	} else {
		digitalWrite(DIODE, LOW);
		analogWrite(BUZZER, 0);
	}
}

void setup() {
	pinMode(DIODE, OUTPUT);
	pinMode(BUZZER, OUTPUT);
	pinMode(BUTTON, INPUT);

	str_init(&codeStr, codeBuffer);
	str_init(&wordStr, wordBuffer);

	isPressed = false;
	wasPressed = false;

	Serial.begin(SERIAL_RATE);
}

void loop() {
	isPressed = digitalRead(BUTTON) == HIGH;
	signal(isPressed ? ON : OFF);

	if (isPressed && !wasPressed) {
		onButtonPressed();
	} else if (!isPressed && wasPressed) {
		onButtonReleased();
	} else if (!isPressed) {
		onButtonIdle();
	}

	wasPressed = isPressed;

	delay(50);
}
