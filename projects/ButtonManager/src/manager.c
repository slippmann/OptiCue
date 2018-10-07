#include "global.h"
#include "power.h"
#include "volume.h"

int main (void)
{
        wiringPiSetupGPIO();

	PowerSetup();
	//VolumeSetup();

	// loop forever
	for(;;)
        {
                delay(500);
        }

        return 0;
}
