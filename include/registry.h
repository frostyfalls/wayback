/* SPDX-License-Identifier: Unlicense */

#ifndef REGISTRY_H
#define REGISTRY_H

/* output_registry.c */
const struct wl_output_listener *output_listener(void);

/* global_registry.c */
const struct wl_registry_listener *registry_listener(void);

/* zwlr_layer_surface_registry.c */
const struct zwlr_layer_surface_v1_listener *zwlr_layer_surface_listener(void);

#endif /* REGISTRY_H */
