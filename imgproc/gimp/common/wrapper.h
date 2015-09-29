#ifndef WRAPPER_H
#define WRAPPER_H


#include <libgimp/gimp.h>

#include "texture.h"


void gimp_drawable_texture_init(texture_t *tex, GimpDrawable *drawable);
void gimp_drawable_texture_free(texture_t *tex);
void gimp_drawable_texture_blit(const texture_t *tex, GimpDrawable *drawable);


#endif
