#include <EnableInterrupt.h>
#include <Adafruit_NeoPixel.h>
#include "Canon.h"

#ifdef __AVR__
#include <avr/power.h>
#endif

#define LEDS_PER_STRIP 12
#define LEDS_PER_CANNON 10
#define NUM_STRIPS 5

#define STRIP_1_PIN 2
#define STRIP_2_PIN 3
#define STRIP_3_PIN 4
#define STRIP_4_PIN 5
#define BTN_PIN 6
#define STRIP_5_PIN 7

#define CANNON_1_PIN 8
#define CANNON_2_PIN 9
#define CANNON_3_PIN 10
#define CANNON_4_PIN 11
#define CANNON_5_PIN 12

#define CANNON_6_PIN 13

enum CanonStates { Off, Charging, Charged, Firing };
CanonStates canonState = Off;

int armPins[NUM_STRIPS] = { STRIP_1_PIN, STRIP_2_PIN, STRIP_3_PIN, STRIP_4_PIN, STRIP_5_PIN };
int cannonPins[NUM_STRIPS] = { CANNON_1_PIN, CANNON_2_PIN, CANNON_3_PIN, CANNON_4_PIN, CANNON_5_PIN };
Canon cannon;

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
	canonState = Off;

	pinMode(BTN_PIN, INPUT);
	enableInterrupt(BTN_PIN, OnButtonPress, RISING);

	cannon.Init(armPins, NUM_STRIPS, LEDS_PER_STRIP, cannonPins, NUM_STRIPS, LEDS_PER_CANNON);
}

void loop() {
	//  colorWipe(strip1, strip1.Color(255, 0, 0), 50); // Red
	if (canonState == Charging)
	{
		cannon.ChargeCannon(300, 50, 15, cannon.Color(127, 50, 50));
		canonState = Charged;
	}
	else if (canonState == Charged)
	{
		cannon.ChargeCannon(50, 50, 1, cannon.Color(127, 50, 50));
	}
	else if (canonState == Firing)
	{
		cannon.FireCanon();
		canonState = Off;
	}
}

void OnButtonPress()
{
	if (canonState == Off)
		canonState = Charging;
	else if (canonState == Charged)
		canonState = Firing;
}

