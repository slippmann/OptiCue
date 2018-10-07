#include "gpio.h"
#include "power.h"

static bool isSleeping = false;

void powerOff(void)
{
#ifdef DEBUG
        piPrint("PowerOff");
#else
	system("sudo shutdown -h now");
#endif
}

void sleep(void)
{
	if(isSleeping)
	{
#ifdef DEBUG
                piPrint("Wake");
#else
                system("sudo python opticue.py");
#endif
	}
	else
	{
#ifdef DEBUG
		piPrint("Sleep");
#else
		system("sudo killall python");
#endif
	}
}

void handlePowerInterrupt(void)
{
	const int waitms = 100;
	bool isLongPress = false;

	int msec = 0;

	if(Debounce(POWER_PIN) == false)
		return; // Ignore

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
		powerOff();
	}
	else
	{
		sleep();
	}
}

void PowerSetup(void)
{
	pinMode(POWER_PIN, INPUT); // Set pin as an input
	pullUpDnControl(POWER_PIN, PUD_UP); // Apply a 50K pullup resistor

	wiringPiISR(POWER_PIN, INT_EDGE_FALLING,  handlePowerInterrupt); // Configure ISR
}
