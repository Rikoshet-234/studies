#include "Arduino.h"

#define LED_R 3
#define LED_G 5
#define LED_B 6

typedef struct {
	unsigned char r, g, b;
} color_t;

void color_setup();
color_t color_new(unsigned char r, unsigned char g, unsigned char b);
color_t color_rand();
void color_show(color_t color);
void color_hide();
