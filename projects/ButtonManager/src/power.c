#include "gpio.h"
#include "proc.h"
#include "power.h"
#include "syswrap.h"

static pthread_t loopThread;
static pthread_mutex_t lock;

static volatile bool isSleeping = true;
static volatile bool isInterrupted = false;

void updateStateLED(void)
{
	if (isSleeping)
	{
		digitalWrite(ACTIVE_LED_PIN, LOW);
		digitalWrite(STANDBY_LED_PIN, HIGH);
	}
	else
	{
		digitalWrite(STANDBY_LED_PIN, LOW);
		digitalWrite(ACTIVE_LED_PIN, HIGH);
	}
}

void powerOff(void)
{
#ifdef DEBUG
        piPrint("PowerOff");
#endif
	systemRun("sudo shutdown -h now");
}

void toggleSleep(void)
{
	bool isRun;

	pthread_mutex_lock(&lock);

	if(isSleeping)
	{
#ifdef DEBUG
                piPrint("Wake");
#endif
		isRun = true;
	}
	else
	{
#ifdef DEBUG
		piPrint("Sleep");
#endif
		isRun = false;
	}

	pthread_mutex_unlock(&lock);

	if(isRun)
		systemRun(START_APP_CMD);
	else
		systemRun(KILL_APP_CMD);
}

void checkAppStatus(void)
{
	bool isFound = (FindProcess(PYTHON_VERSION) != 0);

	pthread_mutex_lock(&lock);

	if(isFound == isSleeping)
	{
		isSleeping = !isSleeping;

		updateStateLED();
	}

	pthread_mutex_unlock(&lock);

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
		delay(500);
		checkAppStatus();
	}

	return NULL;
}

void PowerSetup(void)
{
#ifdef DEBUG
	piPrint("Setting Up Power Manager...");
#endif

	pinMode(STANDBY_LED_PIN, OUTPUT);
	pinMode(ACTIVE_LED_PIN, OUTPUT);

	pinMode(POWER_PIN, INPUT); // Set pin as an input
	pullUpDnControl(POWER_PIN, PUD_UP); // Apply a 50K pullup resistor

	wiringPiISR(POWER_PIN, INT_EDGE_FALLING,  handlePowerInterrupt); // Configure ISR

	pthread_create(&loopThread, NULL, powerLoop, NULL);

#ifdef DEBUG
	piPrint("Power Manager Setup Complete");
#endif
}

void PowerCleanup(void)
{
#ifdef DEBUG
	piPrint("Cleaning Up Power Manager...");
#endif

	isInterrupted = true;

	pinMode(STANDBY_LED_PIN, INPUT);
        pinMode(ACTIVE_LED_PIN, INPUT);

	pinMode(POWER_PIN, INPUT); // Return pin to input mode
	pullUpDnControl(POWER_PIN, PUD_OFF); // Remove pullup

	wiringPiISR(POWER_PIN, INT_EDGE_SETUP, NULL); // Remove interrupt

	pthread_join(loopThread, NULL);

#ifdef DEBUG
	piPrint("Power Manager Clean Up Complete");
#endif
}
