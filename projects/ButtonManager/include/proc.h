#ifndef PROC_H
#define PROC_H

#include "global.h"

#include <dirent.h>
#include <fnmatch.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int FindProcess(char * procName);

#endif /* PROC_H */
