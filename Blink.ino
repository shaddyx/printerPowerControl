/*
 Blink
 Turns on an LED on for one second, then off for one second, repeatedly.

 Most Arduinos have an on-board LED you can control. On the Uno and
 Leonardo, it is attached to digital pin 13. If you're unsure what
 pin the on-board LED is connected to on your Arduino model, check
 the documentation at http://www.arduino.cc

 This example code is in the public domain.

 modified 8 May 2014
 by Scott Fitzgerald
 */
#include <Arduino.h>
#include "states.h"
#define DEBUG 0
#define ON_BUTTON_PIN 2
#define CANCEL_BUTTON_PIN 3
#define LED_PIN 4
#define TURN_ON_PIN 10
#define HOME_PIN 5
#define OFF_DISABLED 10000000

#define TIME_MULTIPLIER 100
#define TIME_TO_WAIT_HOME 15 * 60
//#define TIME_TO_WAIT_HOME 1 * 60
#define TIME_TO_OFF 30
#define TIME_LED 2
#define TIME_TO_FORCE 4

long timer = 0;

int state = STATE_OFF;
int led_state = 0;
int led_timer = 0;
int force_off_timer = 0;
void debug(String s) {
	if (debug) {
		Serial.println(s);
	}
}
int homePosition() {
	return !digitalRead(HOME_PIN);
}
int onButtonPressed() {
	return !digitalRead(ON_BUTTON_PIN);
}
int offButtonPressed() {
	return !digitalRead(CANCEL_BUTTON_PIN);
}

void updateState() {
	int onState = 0;
	if (state == STATE_ON || state == STATE_TURNING_OFF) {
		onState = 1;
	}

	digitalWrite(TURN_ON_PIN, !onState);
	digitalWrite(LED_PIN, led_state);
	debug("ledState: " + String(led_state));
}

// the setup function runs once when you press reset or power the board
void setup() {
	// initialize digital pin 13 as an output.
	if (DEBUG) {
		Serial.begin(115200);
		Serial.println("Started");
	}
	pinMode(LED_PIN, OUTPUT);
	pinMode(TURN_ON_PIN, OUTPUT);
	pinMode(ON_BUTTON_PIN, INPUT);
	pinMode(HOME_PIN, INPUT);
	pinMode(CANCEL_BUTTON_PIN, INPUT);
	digitalWrite(TURN_ON_PIN, HIGH);
	digitalWrite(HOME_PIN, HIGH);
	digitalWrite(ON_BUTTON_PIN, HIGH);
	digitalWrite(CANCEL_BUTTON_PIN, HIGH);
}

void turnOn() {
	state = STATE_ON;
	timer = TIME_TO_WAIT_HOME * TIME_MULTIPLIER;
}

void turnOff() {
	state = STATE_OFF;
}

void startTurningOff() {
	state = STATE_TURNING_OFF;
	timer = TIME_TO_OFF * TIME_MULTIPLIER;
}

void resetTimerStateOn() {
	timer = TIME_TO_WAIT_HOME * TIME_MULTIPLIER;
}
void resetLedTimer() {
	led_timer = TIME_LED * TIME_MULTIPLIER;
}

void updateLed() {
	if (state == STATE_ON) {
		if (homePosition()) {
			led_timer -= 1;
		} else {
			resetLedTimer();
			led_state = 1;
		}
	}

	if (state == STATE_TURNING_OFF) {
		led_timer -= 3;
	}

	if (state == STATE_OFF) {
		resetLedTimer();
		led_state = 0;
	}

	if (led_timer <= 0) {
		resetLedTimer();
		led_state = !led_state;
	}
}

// the loop function runs over and over again forever
void loop() {
	timer--;

	if (timer <= 0) {
		timer = 0;
	}

	if (led_timer) {
		debug("Timer is:" + String(timer));
	}

	if (!homePosition() && state == STATE_ON) {
		resetTimerStateOn();
	}

	if (onButtonPressed()) {
		if (state == STATE_OFF || state == STATE_TURNING_OFF) {
			debug("Turning on");
			turnOn();
		}
	}

	if (offButtonPressed()) {
		if (state == STATE_ON) {
			debug("Starting turn off");
			startTurningOff();
		}
		if (state == STATE_ON || state == STATE_TURNING_OFF) {
			force_off_timer++;
		} else {
			force_off_timer = 0;
		}
	}

	if (force_off_timer >= TIME_TO_FORCE * TIME_MULTIPLIER
			&& (state == STATE_TURNING_OFF || state == STATE_ON)) {
		turnOff();
		force_off_timer = 0;
	}
	if (timer == 0 && state == STATE_TURNING_OFF) {
		debug("Turning off");
		turnOff();
	}

	if (timer == 0 && state == STATE_ON) {
		debug("Starting turn off by timer");
		startTurningOff();
	}

	updateLed();
	updateState();
	delay(10);

}
