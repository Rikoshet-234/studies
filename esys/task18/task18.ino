
#define uint unsigned int
#define ulong unsigned long

#define HALL A0
#define REC_BUTTON 4
#define PLAY_BUTTON 7
#define BUZZER 11


/*
 * HALL TO TONE
 *
 * Here is simple code that simply translates hall sensor measurements to the
 * corresponding tones using very simple formula and some magic constants.
 */

#define MIN_SOUND_FREQUENCY (400 - 100)
#define MAX_SOUND_FREQUENCY (400 + 100)
#define MIN_HALL_VALUE (510 - 30)
#define MAX_HALL_VALUE (510 + 30)

ulong hallToTone(ulong hall) {
	ulong clamped = constrain(hall, MIN_HALL_VALUE, MAX_HALL_VALUE);
	return map(
		clamped,
		MIN_HALL_VALUE, MAX_HALL_VALUE,
		MIN_SOUND_FREQUENCY, MAX_SOUND_FREQUENCY
	);
}


/*
 * RECORDING
 *
 * Stuff here takes care of recording. We simply sample hall value every
 * couple milliseconds and then store this in the array.
 */

#define RECORD_BUFFER_CAPACITY 512
#define RECORD_SAMPLING_FREQUENCY 10

uint rec_buffer[RECORD_BUFFER_CAPACITY];
int rec_bufferSize;
ulong rec_lastSampleTime;

void rec_setup() {
	rec_bufferSize = 0;
	rec_lastSampleTime = 0;
}

void rec_loop() {
	ulong hall = analogRead(HALL);
	tone(BUZZER, hallToTone(hall));

	if (!(millis() - rec_lastSampleTime >= RECORD_SAMPLING_FREQUENCY)) {
		return;
	}

	if (rec_bufferSize < RECORD_BUFFER_CAPACITY) {
		rec_buffer[rec_bufferSize++] = hall;
	}
	rec_lastSampleTime = millis();
}


/*
 * PLAYBACK
 *
 * Playback just goes through recorded values and plays them again as long as
 * there are some notes to play.
 */

int play_currentSample;

void play_setup() {
	play_currentSample = 0;
}

void play_loop() {
	if (play_currentSample < rec_bufferSize) {
		ulong hall = rec_buffer[play_currentSample++];
		tone(BUZZER, hallToTone(hall));
		delay(RECORD_SAMPLING_FREQUENCY);
	} else {
		noTone(BUZZER);
	}
}


/*
 * STATE AUTOMATA
 *
 * Very simple state automata that manages states and fires appropriate
 * methods. Generally speaking, serves as virtual method dispatcher.
 */

#define RECORDING 0
#define PLAYING 1

int state_current;

void state_trans(int state) {
	state_current = state;

	switch (state_current) {
	case RECORDING:
		rec_setup();
		break;
	case PLAYING:
		play_setup();
		break;
	}
}

void state_setup() {
	state_trans(RECORDING);
}

void state_loop() {
	switch (state_current) {
	case RECORDING:
		rec_loop();
		break;
	case PLAYING:
		play_loop();
		break;
	}
}


/*
 * USER INPUT
 *
 * This is the section that deals with user input and runs appropriate actions
 * if required.
 */

bool in_isRecPressed;
bool in_isPlayPressed;

void in_setup() {
	in_isRecPressed = false;
	in_isPlayPressed = false;
}

void in_loop() {
	if (digitalRead(REC_BUTTON) == HIGH) {
		if (!in_isRecPressed) {
			state_trans(RECORDING);
		}
		in_isRecPressed = true;
	} else {
		in_isRecPressed = false;
	}

	if (digitalRead(PLAY_BUTTON) == HIGH) {
		if (!in_isPlayPressed) {
			state_trans(PLAYING);
		}
		in_isPlayPressed = true;
	} else {
		in_isPlayPressed = false;
	}	
}


/*
 * MAIN
 *
 * Entry point of the application.
 */

void setup() {
	pinMode(HALL, INPUT);
	pinMode(REC_BUTTON, INPUT);
	pinMode(PLAY_BUTTON, INPUT);
	pinMode(BUZZER, OUTPUT);

	in_setup();
	state_setup();
}

void loop()  {
	in_loop();
	state_loop();
}
