#ifndef REGISTRY_H
#define REGISTRY_H

/* global_registry.c:
 * Wayland global registry */
const struct wl_registry_listener *registry_listener(void);

/* output_registry.c:
 * Wayland output registry */
const struct wl_output_listener *output_listener(void);

#endif /* REGISTRY_H */
