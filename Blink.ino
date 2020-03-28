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
#include <PMButton.h>
#include <TimerEvent.h>
#include "states.h"
#define DEBUG 0
#define ON_BUTTON_PIN 2
#define CANCEL_BUTTON_PIN 3
#define LED_PIN 4
#define TURN_ON_PIN 10
#define SENSOR_PIN 5
#define OFF_DISABLED 10000000

#define TIME_MULTIPLIER 100
#define TIME_TO_WAIT_HOME 25 * 60
//#define TIME_TO_WAIT_HOME 1 * 60
#define TIME_TO_OFF 30000
#define TIME_LED 2
#define TIME_TO_FORCE 4

long timer = 0;

int state = STATE_OFF;
int led_state = 0;
int led_timer = 0;
int force_off_timer = 0;
int enable_off = 0;

PMButton onButton(ON_BUTTON_PIN);
PMButton cancelButton(CANCEL_BUTTON_PIN);

TimerEvent offTimer;
TimerEvent blinkTimer;


void debug(String s) {
	if (debug) {
		Serial.println(s);
	}
}
int canTurnOff() {
	return digitalRead(SENSOR_PIN);
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
	pinMode(SENSOR_PIN, INPUT);
	pinMode(CANCEL_BUTTON_PIN, INPUT);
	digitalWrite(TURN_ON_PIN, HIGH);
	digitalWrite(SENSOR_PIN, HIGH);
	digitalWrite(ON_BUTTON_PIN, HIGH);
	digitalWrite(CANCEL_BUTTON_PIN, HIGH);
	blinkTimer.set(1000, blinker);
	offTimer.set(TIME_TO_OFF, turnOff);
	blinkTimer.disable();
	offTimer.disable();
}

void blinker(){
	led_state = !led_state;
}

void turnOff(){
	state = STATE_OFF;
}

void turnOn(){
	state = STATE_ON;
}

void startTurningOff(){
	state = STATE_TURNING_OFF;
	offTimer.reset();
	offTimer.enable();
}

void updateLed(){
	if (state = STATE_TURNING_OFF){
		blinkTimer.enable();
	} else {
		blinkTimer.disable();
	}
}



void loop() {
	if (onButton.clicked()){
		turnOn();
	}

	if (cancelButton.clicked() || canTurnOff()){
		startTurningOff();
	}

	if (cancelButton.heldLong()){
		turnOff();
	}

	if (state == STATE_OFF || state == STATE_ON){
		offTimer.disable();
	}

	offTimer.update();
	blinkTimer.update();
	updateLed();
	updateState();
}
