#ifndef REGISTRY_H
#define REGISTRY_H

/* global_registry.c:
 * Wayland global registry */
const struct wl_registry_listener *registry_listener(void);

/* layer_surface_registry.c:
 * Layer surface registry */
const struct zwlr_layer_surface_v1_listener *layer_surface_listener(void);

/* output_registry.c:
 * Wayland output registry */
const struct wl_output_listener *output_listener(void);

#endif /* REGISTRY_H */
