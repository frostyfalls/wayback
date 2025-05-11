/* SPDX-License-Identifier: Unlicense */

#include <wlr-layer-shell-unstable-v1.h>

#include "wayback.h"

static void
zwlr_layer_surface_configure(void *data,
                             struct zwlr_layer_surface_v1 *zwlr_layer_surface,
                             uint32_t serial, uint32_t width, uint32_t height) {
    (void)serial;
    (void)width;
    (void)height;
    struct wayback_output *output = data;

    zwlr_layer_surface_v1_ack_configure(zwlr_layer_surface, serial);

    if (output->configured && output->width == width &&
        output->height == height) {
        wl_surface_commit(output->wl_surface);
        return;
    }

    output->configured = true;
    render_surface(output);
}

static void
zwlr_layer_surface_closed(void *data,
                          struct zwlr_layer_surface_v1 *zwlr_layer_surface) {
    (void)zwlr_layer_surface;
    struct wayback_output *output = data;

    destroy_output_layer(output);
}

static const struct zwlr_layer_surface_v1_listener listener = {
    .configure = zwlr_layer_surface_configure,
    .closed = zwlr_layer_surface_closed,
};

const struct zwlr_layer_surface_v1_listener *zwlr_layer_surface_listener(void) {
    return &listener;
}
