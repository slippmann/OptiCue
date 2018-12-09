#ifndef GLOBAL_H
#define GLOBAL_H

#include <wiringPi.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG

#define LONG_PRESS_DUR_MSEC 	(3000)

#define POWER_PIN		(3)
#define STANDBY_LED_PIN		(17)
#define ACTIVE_LED_PIN		(18)

#define VOLUME_UP_PIN		(27)
#define VOLUME_DOWN_PIN		(22)

#define VOL_FILE		"/usr/local/.OpticueVol"

#define BATTERY_STATUS_PIN	(23)
#define BATTERY_LOW_PIN		(24)
#define CHARGING_LED_PIN	(5)
#define LOW_BATTERY_LED_PIN	(6)

#define PYTHON_VERSION		"python"
#define START_APP_CMD 		"sudo "PYTHON_VERSION" opticue.py &"
#define KILL_APP_CMD		"sudo killall -s SIGINT "PYTHON_VERSION" &"

static void piPrint(const char * format, ...)
{
	va_list args;
    	va_start(args, format);

    	vprintf(format, args);

	va_end(args);

	printf("\n");
}

#endif /* GLOBAL_H */
