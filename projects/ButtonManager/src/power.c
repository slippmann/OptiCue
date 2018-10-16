#include "gpio.h"
#include "power.h"

static bool isSleeping = false;

void updateStateLED(void)
{
	if (isSleeping)
	{
		digitalWrite(ACTIVE_PIN, LOW);
		digitalWrite(STANDBY_PIN, HIGH);
	}
	else
	{
		digitalWrite(STANDBY_PIN, LOW);
		digitalWrite(ACTIVE_PIN, HIGH);
	}
}

void powerOff(void)
{
#ifdef DEBUG
        piPrint("PowerOff");
#endif
	system("sudo shutdown -h now");
}

void sleep(void)
{
	if(isSleeping)
	{
#ifdef DEBUG
                piPrint("Wake");
#endif
                system("sudo python3 opticue.py &");

		isSleeping = false;
	}
	else
	{
#ifdef DEBUG
		piPrint("Sleep");
#endif
		system("sudo killall -s SIGINT python3 &");

		isSleeping = true;
	}

	updateStateLED();
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
	pinMode(STANDBY_PIN, OUTPUT);
	pinMode(ACTIVE_PIN, OUTPUT);

	pinMode(POWER_PIN, INPUT); // Set pin as an input
	pullUpDnControl(POWER_PIN, PUD_UP); // Apply a 50K pullup resistor

	wiringPiISR(POWER_PIN, INT_EDGE_FALLING,  handlePowerInterrupt); // Configure ISR
}

void PowerCleanup(void)
{
	pinMode(STANDBY_PIN, INPUT);
        pinMode(ACTIVE_PIN, INPUT);

	pinMode(POWER_PIN, INPUT); // Return pin to input mode
	pullUpDnControl(POWER_PIN, PUD_OFF); // Remove pullup

	wiringPiISR(POWER_PIN, INT_EDGE_SETUP, NULL); // Remove interrupt
}
