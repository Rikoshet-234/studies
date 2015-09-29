#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>


#define BUTTON_UP 2
#define BUTTON_DOWN 5


#define BINARY_MODE

volatile uint8_t state;


int main()
{
	cli();
	state = 0;

	/*
	 * Setup LED display.
	 *
	 * Fortunately, LED pins are the exactly the DDRB pins (8-13),
	 * so setting it all of them to input output mode is as simple as
	 * setting all bits of DDRB to 1.
	 */ 
	DDRB = ~((uint8_t)0);

	/*
	 * Setup button controls.
	 *
	 * We treat all DDRD pins (except for RX and TX ones) as our input
	 * controls. Therefore, we just clear the whole mask leaving only
	 * original RX (pin 0) and TX (pin 1) values.
	 */
	DDRD = (DDRD & _BV(0)) | (DDRD & _BV(1));

	/*
	 * Setup interrupts.
	 *
	 * The clear button is connected to the pin 3 (interrupt 1) and we want
	 * it to trigger on the falling signal (when use raises his finger).
	 *
	 * Signal of the value keys is combined with diodes and connected to the
	 * pin 2 (interrupt 1). We want interrupt to trigger on the rising signal
	 * to be able to determine which button is being pressed.
	 */
	sei();
	EICRA = (EICRA & ~(_BV(ISC01) | _BV(ISC00))) | _BV(ISC01) | _BV(ISC00);
	EICRA = (EICRA & ~(_BV(ISC11) | _BV(ISC10))) | _BV(ISC11);
	EIMSK = _BV(INT0) | _BV(INT1);


	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_enable();

	for (;;) {
		sleep_mode();
	}
}


#ifdef GRAY_MODE
#define DISPLAY(v) ((v >> 1) ^ v)
#endif

#ifdef BINARY_MODE
#define DISPLAY(v) (v)
#endif


ISR(INT0_vect)
{
	if (PIND & _BV(4)) {
		state++;
	} else if (PIND & _BV(5)) {
		state--;
	}
	PORTB = DISPLAY(state);
}


ISR(INT1_vect)
{
	state = 0;
	PORTB = DISPLAY(state);
}
