#ifndef GPIO_H
#define GPIO_H

#include "global.h"

#define BOUNCE_DUR_MSEC 200

bool Debounce(int pin);

#endif /* GPIO_H */
