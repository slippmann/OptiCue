#include "gpio.h"
#include "proc.h"
#include "power.h"

static pthread_t loopThread;
static pthread_mutex_t lock;

static volatile bool isSleeping = true;
static volatile bool isInterrupted = false;

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

void toggleSleep(void)
{
	pthread_mutex_lock(&lock);

	if(isSleeping)
	{
#ifdef DEBUG
                piPrint("Wake");
#endif
                system(START_APP_CMD);
	}
	else
	{
#ifdef DEBUG
		piPrint("Sleep");
#endif
		system(KILL_APP_CMD);
	}

	pthread_mutex_unlock(&lock);
}

void checkAppStatus(void)
{
	bool isFound = (FindProcess("python3") != 0);

	if(isFound == isSleeping)
	{
		pthread_mutex_lock(&lock);
		isSleeping = !isSleeping;
		pthread_mutex_unlock(&lock);

		updateStateLED();
	}

	return;
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
		toggleSleep();
	}
}

void * powerLoop(void * param)
{
	while(!isInterrupted)
	{
		delay(1000);
		checkAppStatus();
	}

	return NULL;
}

void PowerSetup(void)
{
	pinMode(STANDBY_PIN, OUTPUT);
	pinMode(ACTIVE_PIN, OUTPUT);

	pinMode(POWER_PIN, INPUT); // Set pin as an input
	pullUpDnControl(POWER_PIN, PUD_UP); // Apply a 50K pullup resistor

	wiringPiISR(POWER_PIN, INT_EDGE_FALLING,  handlePowerInterrupt); // Configure ISR

	pthread_create(&loopThread, NULL, powerLoop, NULL);
}

void PowerCleanup(void)
{
	isInterrupted = true;

	pinMode(STANDBY_PIN, INPUT);
        pinMode(ACTIVE_PIN, INPUT);

	pinMode(POWER_PIN, INPUT); // Return pin to input mode
	pullUpDnControl(POWER_PIN, PUD_OFF); // Remove pullup

	wiringPiISR(POWER_PIN, INT_EDGE_SETUP, NULL); // Remove interrupt

	pthread_join(loopThread, NULL);
}
