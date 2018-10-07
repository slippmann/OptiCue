#ifndef GLOBAL_H
#define GLOBAL_H

#include <wiringPi.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

#define DEBUG

#define LONG_PRESS_DUR_MSEC 	(3000)

#define POWER_PIN		(3)
#define VOLUME_UP_PIN		(1)
#define VOLUME_DOWN_PIN		(2)

static void piPrint(const char * format, ...)
{
	va_list args;
    	va_start(args, format);

    	vprintf(format, args);

	va_end(args);

	printf("\n");
}

#endif /* GLOBAL_H */
