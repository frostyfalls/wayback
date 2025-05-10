#include <wayland-client.h>
#include <stdio.h>
#include <wlr-layer-shell-unstable-v1.h>

#include "layer_surface.h"
#include "wayback.h"

static void layer_surface_configure(void *data, struct zwlr_layer_surface_v1 *surface, uint32_t serial, uint32_t width, uint32_t height) {
    struct wayback_output *output = data;

    zwlr_layer_surface_v1_ack_configure(surface, serial);

    if (output->configured && output->render_width == width && output->render_height == height) {
        wl_surface_commit(output->wl_surface);
        return;
    }

    output->render_width = width;
    output->render_height = height;
    output->configured = true;
    render_surface(output);
}

static void layer_surface_closed(void *data, struct zwlr_layer_surface_v1 *surface) {
    (void)surface;
    struct wayback_output *output = data;

    destroy_wayback_output_layer(output);
}

static const struct zwlr_layer_surface_v1_listener listener = {
    .configure = layer_surface_configure,
    .closed = layer_surface_closed,
};

const struct zwlr_layer_surface_v1_listener *layer_surface_listener(void) {
    return &listener;
}
