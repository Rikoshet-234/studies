#include <stdbool.h>

#include "wrapper.h"


void gimp_drawable_texture_init(texture_t *tex, GimpDrawable *drawable)
{
	gint x1, y1, x2, y2;
	gimp_drawable_mask_bounds(drawable->drawable_id, &x1, &y1, &x2, &y2);

	size_t width = x2 - x1;
	size_t height = y2 - y1;
	size_t bpp = gimp_drawable_bpp(drawable->drawable_id);

	texture_alloc(tex, width, height, bpp);

	GimpPixelRgn rgn;
	gimp_pixel_rgn_init(&rgn, drawable,
	                    x1, y1, tex->width, tex->height,
	                    false, false);

	gimp_pixel_rgn_get_rect(&rgn, tex->data,
	                        x1, y1, tex->width, tex->height);
}


void gimp_drawable_texture_free(texture_t *tex)
{
	texture_dealloc(tex);
}


void gimp_drawable_texture_blit(const texture_t *tex, GimpDrawable *drawable)
{
	gint x1, y1, x2, y2;
	gimp_drawable_mask_bounds(drawable->drawable_id, &x1, &y1, &x2, &y2);

	GimpPixelRgn rgn;
	gimp_pixel_rgn_init(&rgn, drawable,
	                    x1, y1, tex->width, tex->height,
	                    true, true);

	gimp_pixel_rgn_set_rect(&rgn, tex->data,
	                        x1, y1, tex->width, tex->height);

	gimp_drawable_merge_shadow(drawable->drawable_id, true);
	gimp_drawable_update(drawable->drawable_id,
	                     x1, y1, tex->width, tex->height);
}
