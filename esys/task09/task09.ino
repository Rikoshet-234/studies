#define uint unsigned int

#define DISPLAY_OFFSET 8
#define DISPLAY_HEIGHT 5
#define CYCLE_DURATION 300

const char IMAGE[] =
	"#  #####  #####"
	"#      #      #"
	"#  #####  #####"
	"#  #          #"
	"#  #####  #####"
;

const uint IMAGE_WIDTH = sizeof(IMAGE) / DISPLAY_HEIGHT;
const uint COLUMN_DURATION = CYCLE_DURATION / IMAGE_WIDTH;
bool rising;
int column;

void setup() {
	for (uint y = 0; y < DISPLAY_HEIGHT; y++) {
		pinMode(DISPLAY_OFFSET + y, OUTPUT);
	}

	rising = true;
	column = 0;
}

void display(uint y) {
	bool on = IMAGE[IMAGE_WIDTH * y + column] != ' ';
	digitalWrite(DISPLAY_OFFSET + y, on ? HIGH : LOW);
}

void loop() {
	if (rising) {
		column++;
		if (column >= IMAGE_WIDTH) {
			rising = false;
			column -= 2;
		}
	} else {
		column--;
		if (column < 0) {
			rising = true;
			column += 2;
		}
	}

	for (int y = 0; y <= DISPLAY_HEIGHT; y++) {
		display(y);
	}

	delayMicroseconds(3500);
}
