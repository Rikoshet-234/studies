#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>

int handle(const char *desc, const int retv);
int oplock(const char *pathname, const int flags);

#define OPTARG(v, short, long) (strcmp(v, short) == 0 || strcmp(v, long) == 0)

#endif