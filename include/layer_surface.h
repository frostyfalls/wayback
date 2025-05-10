#ifndef LAYER_SURFACE_H
#define LAYER_SURFACE_H

#include "wayback.h"

void create_output_surface(struct wayback_output *output);
struct wl_buffer *render_surface(const struct wayback_output *output);

#endif /* LAYER_SURFACE_H */
