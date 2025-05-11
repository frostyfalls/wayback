#include <stdio.h>
#include <string.h>
#include <wayland-client.h>

#include "wayback.h"

static void output_geometry(void *data, struct wl_output *wl_output, int32_t x,
                            int32_t y, int32_t physical_width,
                            int32_t physical_height, int32_t subpixel,
                            const char *make, const char *model,
                            int32_t transform) {
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

    output->make = make != NULL ? strdup(make) : NULL;
    output->model = model != NULL ? strdup(model) : NULL;
}

static void output_mode(void *data, struct wl_output *wl_output, uint32_t flags,
                        int32_t width, int32_t height, int32_t refresh) {
    (void)wl_output;
    (void)flags;
    (void)refresh;
    struct wayback_output *output = data;

    output->width = width;
    output->height = height;
}

static void output_done(void *data, struct wl_output *wl_output) {
    (void)wl_output;
    struct wayback_output *output = data;

    printf("Done configuring output:\n"
           "  - Name: %s %s (%s)\n"
           "  - Resolution: %dx%d\n"
           "  - Scale: %d\n",
           output->make, output->model, output->name, output->width,
           output->height, output->scale);
}

static void output_scale(void *data, struct wl_output *wl_output,
                         int32_t factor) {
    (void)wl_output;
    struct wayback_output *output = data;

    output->scale = factor;
}

static void output_name(void *data, struct wl_output *wl_output,
                        const char *name) {
    (void)wl_output;
    struct wayback_output *output = data;

    free(output->name);

    output->name = name != NULL ? strdup(name) : NULL;
}

static void output_description(void *data, struct wl_output *wl_output,
                               const char *description) {
    (void)data;
    (void)wl_output;
    (void)description;
}

static const struct wl_output_listener listener = {
    .geometry = output_geometry,
    .mode = output_mode,
    .done = output_done,
    .scale = output_scale,
    .name = output_name,
    .description = output_description,
};

const struct wl_output_listener *output_listener(void) {
    return &listener;
}
