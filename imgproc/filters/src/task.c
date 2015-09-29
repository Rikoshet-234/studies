#include "task.h"


void task_a1(filter_t *img, size_t n)
{
	filter_t gauss;
	filter_init(&gauss, n, n);
	filter_binom_2d(&gauss);

	filter_t out;
	filter_init(&out, img->height, img->width);
	filter_conv(img, &gauss, &out);
	filter_copy(&out, img);

	filter_free(&out);
	filter_free(&gauss);
}


void task_a2(filter_t *img, size_t n)
{
	filter_t ver;
	filter_init(&ver, n, 1);
	filter_binom_1d(&ver);

	filter_t hor;
	filter_init(&hor, 1, n);
	filter_binom_1d(&hor);

	filter_t tmp;
	filter_init(&tmp, img->height, img->width);

	filter_conv(img, &ver, &tmp);
	filter_conv(&tmp, &hor, img);

	filter_free(&tmp);
	filter_free(&hor);
	filter_free(&ver);
}


void task_a3(filter_t *img, size_t n)
{
	filter_t ver;
	filter_init(&ver, 3, 1);
	ver.data[0] = ver.data[2] = 1.0f;
	ver.data[1] = 2.0f;
	ver.scale = 0.25f;

	filter_t hor;
	filter_init(&hor, 1, 3);
	hor.data[0] = hor.data[2] = 1.0f;
	hor.data[1] = 2.0f;
	hor.scale = 0.25f;

	filter_t tmp;
	filter_init(&tmp, img->height, img->width);

	for (size_t i = 0; i < (n - 1) / 2; i++) {
		filter_conv(img, &ver, &tmp);
		filter_conv(&tmp, &hor, img);
	}

	filter_free(&tmp);
	filter_free(&hor);
	filter_free(&ver);

	/* Apply box filter of size 2 in case of uneven number. */
	if (n % 2 == 1) {
		task_b2(img, 2);
	}
}


void task_a4(filter_t *img, size_t n)
{
	filter_t gauss;
	filter_init(&gauss, n, n);
	filter_binom_2d(&gauss);

	filter_t out;
	filter_init(&out, img->height, img->width);
	filter_conv_fft(img, &gauss, &out);
	filter_copy(&out, img);

	filter_free(&out);
	filter_free(&gauss);
}


void task_a5(filter_t *img, size_t n)
{
	filter_t ver;
	filter_init(&ver, n, 1);
	filter_binom_1d(&ver);

	filter_t hor;
	filter_init(&hor, 1, n);
	filter_binom_1d(&hor);

	filter_t tmp;
	filter_init(&tmp, img->height, img->width);
	filter_conv_fft(img, &ver, &tmp);
	filter_conv_fft(&tmp, &hor, img);

	filter_free(&tmp);
	filter_free(&hor);
	filter_free(&ver);
}


void task_b1(filter_t *img, size_t n)
{
	task_a2(img, n);
}


void task_b2(filter_t *img, size_t n)
{
	filter_t box_ver;
	filter_init(&box_ver, n, 1);
	filter_box(&box_ver);

	filter_t box_hor;
	filter_init(&box_hor, 1, n);
	filter_box(&box_hor);

	filter_t tmp;
	filter_init(&tmp, img->height, img->width);

	filter_conv(img, &box_ver, &tmp);
	filter_conv(&tmp, &box_hor, img);

	filter_free(&tmp);
	filter_free(&box_hor);
	filter_free(&box_ver);
} 


void task_b3(filter_t *img)
{
	filter_t laplace;
	filter_init(&laplace, 3, 3);
	filter_laplace_2d(&laplace);

	filter_t out;
	filter_init(&out, img->height, img->width);

	filter_conv(img, &laplace, &out);
	filter_copy(&out, img);

	filter_free(&out);
	filter_free(&laplace);

	/* Some values may be out of range, clamp it. */
	for (size_t i = 0; i < img->height * img->width; i++) {
		if (img->data[i] < 0.0f) {
			img->data[i] = 0.0f;
		} else if (img->data[i] > 1.0f) {
			img->data[i] = 1.0f;
		}
	}
}
