#ifndef FILE_HIST_H
#define FILE_HIST_H


#include <stddef.h>
#include <stdint.h>


typedef struct {
	size_t *values;
} hist_t;


void hist_init(hist_t *hist,
               const uint8_t *data, size_t count, uint8_t stride);
void hist_const(hist_t *hist);
void hist_free(hist_t *hist);
void hist_match(const hist_t *patt,
                uint8_t *data, size_t count, uint8_t stride);


#endif
