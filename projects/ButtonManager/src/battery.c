#include "gpio.h"
#include "battery.h"

void updateChargingLED(bool isCharging)
{
#ifdef DEBUG
	if(isCharging)
		piPrint("Charging...");
	else
		piPrint("Charging Stopped...");
#endif
	if (isCharging)
	{
		digitalWrite(BATTERY_STATUS_PIN, HIGH);
	}
	else
	{
		digitalWrite(BATTERY_STATUS_PIN, LOW);
	}
}

void updateLowLED(bool isLow)
{
#ifdef DEBUG
	if(isLow)
		piPrint("Low Battery...");
	else
		piPrint("Battery Okay...");
#endif
	if (isLow)
	{
		digitalWrite(BATTERY_LOW_PIN, HIGH);
	}
	else
	{
		digitalWrite(BATTERY_LOW_PIN, LOW);
	}
}

void handleBatteryInterrupt(void)
{
	bool isCharging = digitalRead(BATTERY_STATUS_PIN);
	bool isLow = digitalRead(BATTERY_LOW_PIN);

	updateChargingLED(isCharging);
	updateLowLED(isLow);
}

void BatterySetup(void)
{
	pinMode(CHARGING_PIN, OUTPUT);
	pinMode(LOW_BATTERY_PIN, OUTPUT);

	pinMode(BATTERY_STATUS_PIN, INPUT); // Set pin as an input
	pinMode(BATTERY_LOW_PIN, INPUT); // Set pin as an input
	pullUpDnControl(BATTERY_STATUS_PIN, PUD_UP); // Apply a 50K pullup resistor
	pullUpDnControl(BATTERY_LOW_PIN, PUD_UP); // Apply a 50K pullup resistor

	wiringPiISR(BATTERY_STATUS_PIN, INT_EDGE_BOTH,  handleBatteryInterrupt); // Configure ISR
	wiringPiISR(BATTERY_LOW_PIN, INT_EDGE_BOTH,  handleBatteryInterrupt); // Configure ISR
}

void batteryCleanup(void)
{
	pinMode(CHARGING_PIN, INPUT);
        pinMode(LOW_BATTERY_PIN, INPUT);

	pinMode(BATTERY_STATUS_PIN, INPUT); // Return pin to input mode
	pinMode(BATTERY_LOW_PIN, INPUT); // Return pin to input mode
	pullUpDnControl(BATTERY_STATUS_PIN, PUD_OFF); // Remove pullup
	pullUpDnControl(BATTERY_LOW_PIN, PUD_OFF); // Remove pullup

	wiringPiISR(BATTERY_STATUS_PIN, INT_EDGE_SETUP, NULL); // Remove interrupt
	wiringPiISR(BATTERY_LOW_PIN, INT_EDGE_SETUP, NULL); // Remove interrupt
}
