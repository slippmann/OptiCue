#include <wiringPi.h>

#define POWER_PIN 0

void PowerSetup(void(*function)(void))
{
	gpioMode(POWER_PIN, INPUT);
}
