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
#include "settings.h"
#include "buttons.h"
#include "tools.h"
#include "timer.h"

byte state = STATE_OFF;
bool enabled = 0;
bool blink_state = 0;
int blink_counter = 0;

int64_t blink_interval = 0;
int64_t off_timer = 0;
int64_t blink_timer = 0;
int64_t test_timer = 0;

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
	pinMode(13, OUTPUT);
	pinMode(CANCEL_BUTTON_PIN, INPUT);
	digitalWrite(TURN_ON_PIN, HIGH);
	digitalWrite(HOME_PIN, HIGH);
	digitalWrite(ON_BUTTON_PIN, HIGH);
	digitalWrite(CANCEL_BUTTON_PIN, HIGH);
	initTimer();

	test_timer = setTimer(1000);
}

void turnOn(){
	state = STATE_ON;
	blink_counter = 0;
	debug("Turning on");
}

void turnOff(){
	state = STATE_OFF;
	blink_counter = 0;
	debug("Turning off");
}

void gracefullOff(){
	off_timer = setTimer(TIME_TO_OFF);
	blink_counter = GRACEFUL_OFF_BLINK_COUNTER;
	blink_interval = GRACEFU_OFF_BLINK_INTERVAL
}

void updateLed(){
	int ledState = 0;
	if (state == STATE_OFF){
		blink_interval = 0;
	}

	if (blink_interval && blink_counter){
		if (checkTimer(&blink_timer)){
			blink_state = !blink_state;
		}

		if (!blink_timer){
			blink_timer = setTimer(blink_interval);
			blink_counter --;
		}
		ledState = blink_state;
	} else if (state == STATE_ON){
		ledState = 1;
	}
	digitalWrite(LED_PIN, ledState);
}

void checkOff(){
	if (checkTimer(&off_timer)){
		turnOff();
	}
}

void updatePower(){
	int onState = state != STATE_OFF;
	digitalWrite(TURN_ON_PIN, !onState);
}

// the loop function runs over and over again forever
char test_state = 0;
void loop() {
	if (checkTimer(&test_timer)){
		test_timer = setTimer(1000);
		test_state = !test_state;
		digitalWrite(13, test_state);

	}
	if (onButtonPressed()){
		if (state == STATE_OFF){
			turnOn();
		}
		if (off_timer){
			off_timer = getTime();
		}
	}

	if (getOutOfHomeHold() >= TIME_TO_ENABLE){
		enabled = 1;
		blink_counter = ENABLE_BLINK_COUNTER;
		blink_interval = ENABLE_BLINK_INTERVAL
	}

	if (getOffHold() >= TIME_TO_FORCE){
		turnOff();
	}

	if (offButtonPressed()){
		blink_counter = TURNING_OFF_BLINK_COUNTER;
		blink_interval = TURNING_OFF_BLINK_INTERVAL
		gracefullOff();
	}

	if (enabled && getHomeHold() >= TIME_TO_WAIT_HOME){
		gracefullOff();
	}

	updateLed();
	pollTimer();
	pollButtons();
	checkOff();
	updatePower();
	delay(1);
}
