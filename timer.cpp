/*
 * timer.c
 *
 *  Created on: Feb 26, 2014
 *      Author: shaddy
 */
#include <Arduino.h>
#include "timer.h"
#include "util.h"
#define maxVal 4294967295

int64_t counter = 0;
int64_t lastValue = 0;
char timerNotStarted = 1;


void initTimer() {
}

void pollTimer(){
	unsigned int curValue = millis();
	unsigned int delta;
	if (curValue >= lastValue) {
		delta = curValue - lastValue;
	} else {
		delta = millis() + (maxVal - lastValue);
	}
	lastValue = curValue;
	counter += delta;
}

int64_t getTime() {
	pollTimer();
	return counter;
}

char checkTimer(int64_t* timer) {
	pollTimer();
	if (*timer != 0 && *timer <= counter) {
		*timer = 0;
		return 1;
	}
	return 0;
}

int64_t setTimer(int64_t ms){
	pollTimer();
	return counter + ms;
}
