#ifndef FILE_UTILITY_H
#define FILE_UTILITY_H


#include "filter.h"


bool filter_load(filter_t *filter, const char *path);
void filter_save(const filter_t *filter, const char *path);


#endif
