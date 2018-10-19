#ifndef POWER_H
#define POWER_H

#include <wiringPi.h>
#include <pthread.h>
#include "global.h"

void PowerSetup(void);
void PowerCleanup(void);

#endif /* POWER_H */
