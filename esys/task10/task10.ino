/* Useful defines. */
#define uint unsigned int
#define ulong unsigned long

float randomFloat(float a, float b) {
	const long SAMPLES = 10000;
	return (float)random(a * SAMPLES, b * SAMPLES) / (float)SAMPLES;
}

/* Pin configuration. */
#define BUZZER 3
#define LDR A0

/* Music player utilites. */
static const char NOTES[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
static const uint FREQS[] =  { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
static uint TONES[sizeof(char) * 8];

static float volume;
static float speed;

void mp_init() {
	uint noteCount = min(sizeof NOTES, sizeof TONES);
	for (uint i = 0; i < noteCount; i++) {
		TONES[NOTES[i]] = FREQS[i];
	}

	volume = 1.0f;
	speed = 1.0f;
}

void mp_setVolume(float value) {
	volume = value;
}

void mp_setSpeed(float value) {
	speed = value;
}

void mp_playNote(char note, uint duration) {
	ulong uduration = duration * 1000UL;
	uint tone = (uint)(volume * TONES[note] + 0.5f);
	
	for (ulong i = 0; i < uduration; i += tone * 2) {
		digitalWrite(BUZZER, HIGH);
		delayMicroseconds(tone);
		digitalWrite(BUZZER, LOW);
		delayMicroseconds(tone);
	}
}

void mp_playMelody(const char *melody, uint tempo) {
	while (*melody != '\0') {
		uint beat = *melody - '0';
		char note = *(melody + 1);

		if (note == ' ') {
			delay((ulong)(0.5f + beat * tempo / speed));
		} else {
			mp_playNote(note, (uint)(0.5f + beat * tempo / speed));
		}

		delay((ulong)(0.5f + tempo / speed) / 2);

		melody += 2;
	}
}

/* Program settings. */
#define MEASUREMENT_DURATION 50
#define MEASUREMENT_TOLERANCE 200
#define DEFAULT_TEMPO 300

#define MIN_VOLUME 0.8
#define MAX_VOLUME 1.2

#define MIN_SPEED 0.5
#define MAX_SPEED 2.0

/* Program logic. */
static const char MORNING_MELODY[] =
	"1c1c1g1 ";
static const char OTHER_MELODY[] =
	"2d2a1f2c2d2a2d2c2f2d2a2c2d2a1f2c2d2a2a2g2p8p8p8p";

ulong measurementStartTime;
int ldrMin, ldrMax;


void setup() {
	Serial.begin(9600);
	randomSeed(analogRead(A5));

	mp_init();

	pinMode(BUZZER, OUTPUT);
	pinMode(LDR, INPUT);

	measurementStartTime = 0;
	ldrMin = ldrMax = analogRead(LDR);
	mp_setVolume(randomFloat(MIN_VOLUME, MAX_VOLUME));
	mp_setSpeed(randomFloat(MIN_VOLUME, MAX_VOLUME));
}

void loop() {
	int ldr = analogRead(LDR);
	ldrMin = min(ldrMin, ldr);
	ldrMax = max(ldrMax, ldr);

	ulong currentTime = millis();
	if (currentTime - measurementStartTime >= MEASUREMENT_DURATION) {
		if (abs(ldrMax - ldrMin) >= MEASUREMENT_TOLERANCE) {
			Serial.println(abs(ldrMax - ldrMin));
			mp_playMelody(MORNING_MELODY, DEFAULT_TEMPO);
		}

		/*
		 * Reset light measurement details. Since the melody player blocks
		 * the thread, we can't use old time and LDR values and they have
		 * to be read again.
		 */
		ldrMin = ldrMax = analogRead(LDR);
		measurementStartTime = millis();
	}
}
