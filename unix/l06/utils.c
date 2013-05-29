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
	
	int retv = lockf(fd, F_LOCK, 0);
	if (retv < 0) {
		return retv;
	}

	return fd;
}

int match(const char *string, char *pattern)
{
	regex_t re;
	if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) {
		return 0;
	}

	int status = regexec(&re, string, (size_t) 0, NULL, 0);
	regfree(&re);
	if (status != 0) {
		return 0;
	}

	return 1;
}
