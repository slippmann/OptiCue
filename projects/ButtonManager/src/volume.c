#include "gpio.h"
#include "volume.h"
#include "syswrap.h"

static int currentVolume = 50;

void readVol()
{
	char command[28] = {0};
	FILE * fp;
        fp = fopen(VOL_FILE,"r");

        if(fp == NULL)
        {
                currentVolume = 50;
                return;
        }

        fscanf(fp, "%d", &currentVolume);
        fclose(fp);

	snprintf(command, 25, "sudo amixer set PCM %d%% &", currentVolume);
        systemRun(command);
}

void writeVol()
{
	FILE * fp;
        fp = fopen(VOL_FILE,"w");

	if(fp == NULL)
	{
		piPrint("ERROR opening volume file");
		return;
	}

	fprintf(fp, "%d", currentVolume);
	fclose(fp);
}

void adjustVolume(enum Volume dir)
{
	char command[28] = {0};

	if(dir == UP)
	{
		if(currentVolume < 100)
			currentVolume += 10;
#ifdef DEBUG
        	piPrint("Volume: +10");
#endif
	}
	else
	{
		if(currentVolume > 0)
			currentVolume -= 10;
#ifdef DEBUG
        	piPrint("Volume: -10");
#endif
	}

	snprintf(command, 25, "sudo amixer set PCM %d%% &", currentVolume);
	systemRun(command);

	writeVol();
}

void handleVolumeInterrupt(void)
{
	int pin = 0;

	if(digitalRead(VOLUME_UP_PIN) == LOW)
		pin = VOLUME_UP_PIN;
	else
		pin = VOLUME_DOWN_PIN;

	if(Debounce(pin) == false)
		return; // Ignore

	if(pin == VOLUME_UP_PIN)
		adjustVolume(UP);
	else
		adjustVolume(DOWN);
}

void VolumeSetup(void)
{
#ifdef DEBUG
        piPrint("Setting Up Volume Manager...");
#endif

	pinMode(VOLUME_UP_PIN, INPUT); // Set pin as an input
	pinMode(VOLUME_DOWN_PIN, INPUT); // Set pin as an input
	pullUpDnControl(VOLUME_UP_PIN, PUD_UP); // Apply a 50K pullup resistor
	pullUpDnControl(VOLUME_DOWN_PIN, PUD_UP); // Apply a 50K pullup resistor

	wiringPiISR(VOLUME_UP_PIN, INT_EDGE_FALLING,  handleVolumeInterrupt); // Configure ISR
	wiringPiISR(VOLUME_DOWN_PIN, INT_EDGE_FALLING,  handleVolumeInterrupt); // Configure ISR

	readVol();
#ifdef DEBUG
        piPrint("Volume Manager Setup Complete");
#endif
}

void VolumeCleanup(void)
{
#ifdef DEBUG
        piPrint("Cleaning Up Volume Manager...");
#endif

	pinMode(VOLUME_UP_PIN, INPUT); // Return pin to input mode
	pinMode(VOLUME_DOWN_PIN, INPUT); // Return pin to input mode
	pullUpDnControl(VOLUME_UP_PIN, PUD_OFF); // Remove pullup
	pullUpDnControl(VOLUME_DOWN_PIN, PUD_OFF); // Remove pullup

	wiringPiISR(VOLUME_UP_PIN, INT_EDGE_SETUP, NULL); // Remove interrupt
	wiringPiISR(VOLUME_DOWN_PIN, INT_EDGE_SETUP, NULL); // Remove interrupt

	writeVol();
#ifdef DEBUG
        piPrint("Volume Manager Clean Up Complete");
#endif
}
