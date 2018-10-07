#include "global.h"
#include "power.h"
//#include "volume.h"

int main (void)
{
#ifdef DEBUG
	piPrint("Starting...");
#endif
	wiringPiSetupGpio();

	PowerSetup();
	//VolumeSetup();

	// loop forever
	for(;;)
        {
                delay(500);
        }

        return 0;
}
