/* Pin configuration. */
#define BUTTON 2
#define DIODE 6

/* Program settings. */
#define DIODE_DELAY 1000
#define BUFFER_CAPACITY 128

/* Button press intervals buffer. */
unsigned long buffer[BUFFER_CAPACITY];
unsigned int bufferBegin;
unsigned int bufferSize;

unsigned long *bufferAt(int i) {
	return buffer + (bufferBegin + i) % BUFFER_CAPACITY;
}

void bufferPut(unsigned long value) {
	*bufferAt(bufferSize) = value;
	bufferSize++;
}

unsigned long bufferPeek() {
	return *bufferAt(0);
}

unsigned long bufferPop() {
	bufferBegin++;
	bufferSize--;
}

/* Main program logic. */
boolean diodeState;
boolean buttonState;

void setup() {
	pinMode(BUTTON, INPUT);
	pinMode(DIODE, OUTPUT);
 
	bufferBegin = 0;
	bufferSize = 0;
	
	diodeState = 0;
	buttonState = 0;
}

void loop() {
	/*
	 * First, we check whether the user changed the button state. If he did, we
	 * put the store current time in the buffer.
	 */  
	boolean newButtonState = digitalRead(BUTTON) == HIGH;
	if (newButtonState != buttonState) {
		bufferPut(millis());
	}
	buttonState = newButtonState;

	/*
	 * Now, we check whether the value at the beginning of the buffer is
	 * outdated (that is: older than current time after the specified delay).
	 * If so, we need to flip the diode state.
	 */
	if (bufferSize > 0 && bufferPeek() + DIODE_DELAY <= millis()) {
		bufferPop();
		diodeState = !diodeState;
	}  
	digitalWrite(DIODE, diodeState ? HIGH : LOW);

	delay(10);
}
