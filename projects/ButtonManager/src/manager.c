#include <signal.h>

#include "global.h"
#include "power.h"
#include "volume.h"
#include "battery.h"
#include "syswrap.h"

static volatile bool isRunning = true;

void intHandler(int param)
{
#ifdef DEBUG
	piPrint("Cleaning up...");
#endif
	PowerCleanup();
	VolumeCleanup();
	BatteryCleanup();

	isRunning = false;
}

void StartApp(void)
{
#ifdef DEBUG
	piPrint("Running Opticue...");
#endif
	systemRun(START_APP_CMD);

#ifdef DEBUG
	piPrint("Opticue Started");
#endif
}

int main (int argc, char *argv[])
{
#ifdef DEBUG
	piPrint("Starting...");
#endif
	// Set up signal handlers
	signal(SIGINT, intHandler);
	signal(SIGTERM, intHandler);
	signal(SIGKILL, intHandler);

	StartApp();

	// Initialize GPIOs
	wiringPiSetupGpio();

	PowerSetup();
	VolumeSetup();
	BatterySetup();

	// loop until killed
	while(isRunning)
        {
                delay(500);
        }

        return 0;
}
