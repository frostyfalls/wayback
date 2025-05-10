#include <stdlib.h>

#include "wayback.h"

bool parse_color(const char *color_string, struct wayback_output_config *config) {
    uint32_t color_uint = strtol(color_string, NULL, 16);
    if (color_uint == 0) {
        return false;
    }
    config->color = color_uint;
    return true;
}

void destroy_wayback_output_layer(struct wayback_output *output) {
    if (output->zwlr_layer_surface != NULL) {
        zwlr_layer_surface_v1_destroy(output->zwlr_layer_surface);
        output->zwlr_layer_surface = NULL;
    }
    if (output->wl_surface != NULL) {
        wl_surface_destroy(output->wl_surface);
        output->wl_surface = NULL;
    }
    output->configured = false;
}

void destroy_wayback_output(struct wayback_output *output) {
    destroy_wayback_output_layer(output);

    /* See `Detailed Description` in man wl_list(3). */
    wl_list_init(&output->link);
    wl_list_remove(&output->link);

    if (output->wl_output != NULL) {
        wl_output_destroy(output->wl_output);
        output->wl_output = NULL;
    }

    free(output->make);
    free(output->model);
    free(output);
}
