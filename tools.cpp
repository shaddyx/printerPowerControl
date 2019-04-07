#include <Arduino.h>
#include "settings.h"

void debug(String s) {
	if (debug) {
		Serial.println(s);
	}
}
