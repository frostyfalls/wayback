#include <wayland-client.h>
#include <wlr-layer-shell-unstable-v1.h>

#include "wayback.h"

void destroy_output_layer(struct wayback_output *output) {
    if (output->wl_surface != NULL) {
        wl_surface_destroy(output->wl_surface);
        output->wl_surface = NULL;
    }
    if (output->zwlr_layer_surface != NULL) {
        zwlr_layer_surface_v1_destroy(output->zwlr_layer_surface);
        output->zwlr_layer_surface = NULL;
    }
    output->configured = false;
}

void destroy_output(struct wayback_output *output) {
    if (output->wl_output != NULL) {
        wl_output_release(output->wl_output);
        output->wl_output = NULL;
    }
    free(output->make);
    free(output->model);
    free(output->name);
    free(output);
}
