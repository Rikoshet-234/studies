#ifndef __UTILS_H
#define __UTILS_H

#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <regex.h>

#include <sys/file.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int handle(const char *desc, const int retv);
int oplock(const char *pathname, const int flags);
int match(const char *string, char *pattern);

#define OPTARG(v, short, long) (strcmp(v, short) == 0 || strcmp(v, long) == 0)

#endif
