#include <stdbool.h>
#include <stdlib.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#include "power.h"

static bool isSleeping = false;

void handlePowerInterrupt(void)
{
	const int waitms = 100;
	bool isLongPress = false;

	int msec = 0;

	while(digitalRead(POWER_PIN) == LOW)
	{
		if(msec >= LONG_PRESS_DUR_MSEC)
                {
                        isLongPress = true;
                        break;
                }

		delay(waitms);
		msec += waitms;
	}

	if(isLongPress)
	{
		PowerOff();
	}
	else
	{
		Sleep();
	}
}

void PowerSetup(void)
{
	pinMode(POWER_PIN, INPUT); // Set pin as an input
	pullUpDnControl(POWER_PIN, PUD_UP); // Apply a 50K pullup resistor

	wiringPiISR(POWER_PIN, INT_EDGE_FALLING,  handlePowerInterrupt); // Configure ISR
}

void PowerOff(void)
{
#ifdef DEBUG
        printf("PowerOff");
#else
	system("sudo shutdown -h now");
#endif
}

void Sleep(void)
{
	if(isSleeping)
	{
#ifdef DEBUG
                printf("Wake");
#else
                system("sudo python opticue.py");
#endif
	}
	else
	{
#ifdef DEBUG
		printf("Sleep");
#else
		system("sudo killall python");
#endif
	}
}
