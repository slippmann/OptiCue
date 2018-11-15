#ifndef BATTERY_H
#define BATTERY_H

#include <wiringPi.h>
#include <pthread.h>
#include "global.h"

void BatterySetup(void);
void BatteryCleanup(void);

#endif /* BATTERY_H */
