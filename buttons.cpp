#include "settings.h"
#include <Arduino.h>
#include "buttons.h"
#include "timer.h"


unsigned long onButtonHold = 0;
unsigned long offButtonHold = 0;
unsigned long homeHold = 0;
unsigned long outOfHomeHold = 0;
unsigned long oldTime = 0;


int homePosition() {
	return !digitalRead(HOME_PIN);
}
int onButtonPressed() {
	return !digitalRead(ON_BUTTON_PIN);
}
int offButtonPressed() {
	return !digitalRead(CANCEL_BUTTON_PIN);
}

unsigned long getHomeHold(){
	return homeHold;
}
unsigned long getOutOfHomeHold(){
	return outOfHomeHold;
}

unsigned long getonHold(){
	return onButtonHold;
}

unsigned long getOffHold(){
	return offButtonHold;
}

void pollButtons(){
	if (oldTime == 0){
		oldTime = getTime();
	}
	int timeDiff = getTime() - oldTime;
	if (onButtonPressed()){
		onButtonHold += timeDiff;
	} else {
		onButtonHold = 0;
	}

	if (offButtonPressed()){
		offButtonHold += timeDiff;
	} else {
		offButtonHold = 0;
	}

	if (homePosition()){
		homeHold += timeDiff;
		outOfHomeHold = 0;
	} else {
		homeHold = 0;
		outOfHomeHold += timeDiff;
	}

	oldTime = getTime();
}
