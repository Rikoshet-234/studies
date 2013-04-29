#include "utils.h"

int handle(const char *desc, const int retv)
{
	if (retv < 0) {
		printf("%3d - %s (%s)\n", errno, strerror(errno), desc);
		exit(1);
	}
	return retv;
}

int oplock(const char *pathname, const int flags)
{
	int fd = open(pathname, flags);
	if (fd < 0) {
		return fd;
	}
	
	int retv = flock(fd, F_LOCK, 0);
	if (retv < 0) {
		return retv;
	}

	return fd;
}