#include <Arduino.h>
#include "settings.h"

void debug(String s) {
	if (DEBUG) {
		Serial.println(s);
	}
}
