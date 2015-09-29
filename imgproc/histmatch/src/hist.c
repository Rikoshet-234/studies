#include "hist.h"


#include <stdlib.h>
#include <math.h>


void hist_init(hist_t *hist, const uint8_t *data, size_t count, uint8_t stride)
{
	hist->values = (size_t *)malloc(256 * sizeof(size_t));

	for (size_t i = 0; i < 256; i++) {
		hist->values[i] = 0;
	}
	for (size_t i = 0; i < count; i++) {
		hist->values[data[i * stride]]++;
	}
}


void hist_const(hist_t *hist)
{
	hist->values = (size_t *)malloc(256 * sizeof(size_t));
	for (size_t i = 0; i < 256; i++) {
		hist->values[i] = 1;
	}
}


void hist_free(hist_t *hist)
{
	free(hist->values);
}


static void hist_transform(const float *scdf, const float *pcdf, uint8_t *pixel)
{
	size_t m = *pixel;
	for (size_t i = 0; i < 255; i++) {
		/* Option 1. */
		/*
		float lower = pcdf[i];
		float upper = pcdf[i + 1];
		if (lower <= scdf[*pixel] && scdf[*pixel] <= upper) {
			float factor = (upper - lower) / (scdf[*pixel] - lower);
			*pixel = i + (uint8_t)(factor + 0.5f);
			break;
		}
		*/

		/* Option 2. */
		float cdiff = fabs(scdf[*pixel] - pcdf[m]);
		float idiff = fabs(scdf[*pixel] - pcdf[i]);
		if (idiff < cdiff ||
			(idiff == cdiff && abs(*pixel - i) < abs(*pixel - m)))
		{
			m = i;
		}
	}
	*pixel = m;
}


void hist_match(const hist_t *patt,
                uint8_t *data, size_t count, uint8_t stride)
{
	hist_t hist;
	hist_init(&hist, data, count, stride);

	float *scdf = (float *)malloc(256 * sizeof(float));
	float *pcdf = (float *)malloc(256 * sizeof(float));

	scdf[0] = (float)hist.values[0];
	pcdf[0] = (float)patt->values[0];
	for (size_t i = 1; i < 256; i++) {
		scdf[i] = scdf[i - 1] + (float)hist.values[i];
		pcdf[i] = pcdf[i - 1] + (float)patt->values[i];
	}
	for (size_t i = 0; i < 256; i++) {
		scdf[i] = scdf[i] / (float)scdf[255];
		pcdf[i] = pcdf[i] / (float)pcdf[255];
	}

	uint8_t *transform = (uint8_t *)malloc(256 * sizeof(uint8_t));

	for (size_t i = 0; i < 256; i++) {
		transform[i] = i;
		hist_transform(scdf, pcdf, &transform[i]);
	}

	for (size_t i = 0; i < count; i++) {
		data[i * stride] = transform[data[i * stride]];
	}

	free(transform);

	free(scdf);
	free(pcdf);

	hist_free(&hist);
}
