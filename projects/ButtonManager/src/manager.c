#include <signal.h>

#include "global.h"
#include "power.h"
//#include "volume.h"

static volatile bool isRunning = true;

void intHandler(int param)
{
#ifdef DEBUG
	piPrint("Cleaning up...");
#endif
	PowerCleanup();
	//VolumeCleanup();

	isRunning = false;
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

	// Initialize GPIOs
	wiringPiSetupGpio();

	PowerSetup();
	//VolumeSetup();

	// loop until killed
	while(isRunning)
        {
                delay(500);
        }

        return 0;
}
