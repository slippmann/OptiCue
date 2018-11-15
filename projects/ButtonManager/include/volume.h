#ifndef VOLUME_H
#define VOLUME_H

#include <wiringPi.h>
#include <pthread.h>
#include "global.h"

enum Volume {
	UP,
	DOWN
};

void VolumeSetup(void);
void VolumeCleanup(void);

#endif /* VOLUME_H */
