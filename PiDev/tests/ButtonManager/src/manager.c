#include <wiringPi.h>
#include "power.h"
#include "volume.h"

void powerPinHandler(void);
void volumePinHandler(void);

int main (void)
{
        wiringPiSetup();
        powerSetup(powerPinHandler);
	volumeSetup(volumePinHandler);

	// loop forever
	for(;;)
        {
                delay(500);
        }

        return 0;
}

void powerPinHandler(void)
{
	int timer = 0;

	while(timer < 3000)
	{
		if(!PowerPressed)
		{
			Sleep();
			return;
		}

		delayMicroseconds(100);
		timer += 100;
	}

	PowerOff();
}
