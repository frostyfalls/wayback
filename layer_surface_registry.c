#include <wayland-client.h>
#include <wlr-layer-shell-unstable-v1.h>

#include "wayback.h"

static void layer_surface_configure(void *data, struct zwlr_layer_surface_v1 *surface, uint32_t serial, uint32_t width, uint32_t height) {
    (void)surface;
    struct wayback_output *output = data;

    output->width = width;
    output->height = height;
    output->needs_ack = true;
    output->dirty = true;
    output->configure_serial = serial;
    zwlr_layer_surface_v1_ack_configure(output->zwlr_layer_surface, serial);
    wl_surface_commit(output->wl_surface);
}

static void layer_surface_closed(void *data, struct zwlr_layer_surface_v1 *surface) {
    (void)surface;
    struct wayback_output *output = data;

    destroy_wayback_output(output);
}

static const struct zwlr_layer_surface_v1_listener listener = {
    .configure = layer_surface_configure,
    .closed = layer_surface_closed,
};

const struct zwlr_layer_surface_v1_listener *layer_surface_listener(void) {
    return &listener;
}
