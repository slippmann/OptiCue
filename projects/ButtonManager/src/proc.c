#include "proc.h"

extern int scandir(const char *dirp, struct dirent ***namelist,
              int (*filter)(const struct dirent *),
              int (*compar)(const struct dirent **, const struct dirent **));

int processdir(const struct dirent * dir, char * name, int nameLength)
{
	int len = strlen(dir->d_name) + 15;
	char path[len];

	snprintf(path, len, "/proc/%s/cmdline", dir->d_name);

	FILE *f = fopen(path, "r");

	if(f == NULL)
	{
		perror("Error opening file");
		return -1;
	}

	fgets(name, nameLength, f);
	fclose(f);

	return 0;
}

int filter(const struct dirent *dir)
{
	uid_t user;
	struct stat dirinfo;
	int len = strlen(dir->d_name) + 7;
	char path[len];

	strcpy(path, "/proc/");
	strcat(path, dir->d_name);
	user = getuid();

	if (stat(path, &dirinfo) < 0)
	{
		perror("processdir() ==> stat()");
		return -1;
	}

	return !fnmatch("[1-9]*", dir->d_name, 0) && user == dirinfo.st_uid;
}

int FindProcess(char * procName)
{
	struct dirent **namelist;
	int n = 0, pid = 0;
	char name[32];
	bool isFound = false;

	n = scandir("/proc", &namelist, filter, 0);

	if (n < 0)
	{
		perror("Not enough memory.");
	}
	else
	{
		while(n--)
		{
			if(!isFound)
			{
				if(processdir(namelist[n], name, 32))
				{
					free(namelist[n]);
					continue;
				}

				if (!strcmp(name, procName))
				{
					pid = atoi(namelist[n]->d_name);
#ifdef DEBUG
					printf("PID: %d\n", pid);
#endif
					isFound = true;
				}
			}

			free(namelist[n]);
		}

		free(namelist);
	}

	return pid;
}
