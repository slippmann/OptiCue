#include "syswrap.h"
#include "pthread.h"

void * runner(void * data)
{
	char * cmd = (char*)data;
	char cmdcpy[32];
        snprintf(cmdcpy, 32, "%s", cmd);

	system(cmdcpy);
	return NULL;
}

void systemRun(char * cmd)
{
	pthread_t sysThread;
	pthread_create(&sysThread, NULL, runner, (void*)cmd);
	pthread_join(sysThread, NULL);
}
