#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wayland-client.h>

#include "layer_surface.h"
#include "wayback.h"

static void output_geometry(void *data, struct wl_output *wl_output, int32_t x, int32_t y, int32_t physical_width, int32_t physical_height, int32_t subpixel, const char *make, const char *model, int32_t transform) {
    (void)wl_output;
    (void)x;
    (void)y;
    (void)physical_width;
    (void)physical_height;
    (void)subpixel;
    (void)transform;
    struct wayback_output *output = data;

    free(output->make);
    free(output->model);

    if (make != NULL) {
        uint32_t len = strlen(make) + 1;
        char *new_make = malloc(len);
        if (new_make != NULL) {
            memcpy(new_make, make, len);
        }
        output->make = new_make;
    }
    if (model != NULL) {
        uint32_t len = strlen(model) + 1;
        char *new_model = malloc(len);
        if (new_model != NULL) {
            memcpy(new_model, model, len);
        }
        output->model = new_model;
    }
}

static void output_mode(void *data, struct wl_output *wl_output, uint32_t flags, int32_t width, int32_t height, int32_t refresh) {
    (void)wl_output;
    (void)refresh;

    if ((flags & WL_OUTPUT_MODE_CURRENT) == 0) {
        return;
    }

    struct wayback_output *output = data;
    output->width = width;
    output->height = height;
}

static void output_done(void *data, struct wl_output *wl_output) {
    (void)wl_output;
    struct wayback_output *output = data;

    printf("Done configuring output:\n"
           "  - Name: %s %s\n"
           "  - Resolution: %dx%d\n"
           "  - Scale: %d\n",
        output->make, output->model, output->width, output->height, output->scale);
}

static void output_scale(void *data, struct wl_output *wl_output, int32_t factor) {
    (void)data;
    (void)wl_output;
    struct wayback_output *output = data;

    output->scale = factor;

    if (output->configured) {
        render_surface(output);
    }
}

static const struct wl_output_listener listener = {
    .geometry = output_geometry,
    .mode = output_mode,
    .done = output_done,
    .scale = output_scale,
};

const struct wl_output_listener *output_listener(void) {
    return &listener;
}
