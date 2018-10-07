#include "gpio.h"

bool Debounce(int pin)
{
	delay(BOUNCE_DUR_MSEC);

	return (digitalRead(pin) == LOW);
}
