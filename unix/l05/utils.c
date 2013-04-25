#include "utils.h"

int handle(const char *desc, int retv)
{
	if (retv < 0) {
		printf("%3d - %s (%s)\n", errno, strerror(errno), desc);
		exit(1);
	}
	return retv;
}
