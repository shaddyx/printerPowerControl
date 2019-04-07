#include "settings.h"
#include <Arduino.h>
#include "buttons.h"

int homePosition() {
	return !digitalRead(HOME_PIN);
}
int onButtonPressed() {
	return !digitalRead(ON_BUTTON_PIN);
}
int offButtonPressed() {
	return !digitalRead(CANCEL_BUTTON_PIN);
}
