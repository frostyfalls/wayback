#include <wayland-client.h>

#include "registry.h"
#include "wayback.h"

#define LAYER_SURFACE_DESCRIPTION "wallpaper"

/* TODO: Split render into 2 functions: draw and render */
void render_surface(const struct wayback_output *output) {
    struct wayback_buffer buffer;

    if (!create_buffer(&buffer, output->state->wl_shm, output->render_width * output->scale, output->render_height *output->scale, WL_SHM_FORMAT_XRGB8888)) {
        return;
    }

    /* https://wayland-book.com/surfaces/shared-memory.html#creating-buffers-from-a-pool */
    uint32_t *pool_data_uint = (uint32_t *)buffer.pool_data;
    uint32_t *pixels = &pool_data_uint[0];
    for (uint32_t y = 0; y < output->height; y++) {
        for (uint32_t x = 0; x < output->width; x++) {
            if ((x + y / 8 * 8) % 16 < 8) {
                pixels[y * output->width + x] = 0xFF666666;
            } else {
                pixels[y * output->width + x] = 0xFFEEEEEE;
            }
        }
    }

    wl_surface_set_buffer_scale(output->wl_surface, output->scale);
    wl_surface_attach(output->wl_surface, buffer.wl_buffer, 0, 0);
    wl_surface_damage_buffer(output->wl_surface, 0, 0, output->width * output->scale, output->height * output->scale);
    wl_surface_commit(output->wl_surface);

    destroy_buffer(&buffer);
}

void create_output_surface(struct wayback_output *output) {
    output->wl_surface = wl_compositor_create_surface(output->state->wl_compositor);

    struct wl_region *input_region = wl_compositor_create_region(output->state->wl_compositor);
    wl_surface_set_input_region(output->wl_surface, input_region);
    wl_region_destroy(input_region);

    struct wl_region *opaque_region = wl_compositor_create_region(output->state->wl_compositor);
    wl_surface_set_opaque_region(output->wl_surface, opaque_region);
    wl_region_destroy(opaque_region);

    output->zwlr_layer_surface = zwlr_layer_shell_v1_get_layer_surface(output->state->zwlr_layer_shell, output->wl_surface, output->wl_output, ZWLR_LAYER_SHELL_V1_LAYER_BOTTOM, LAYER_SURFACE_DESCRIPTION);

    zwlr_layer_surface_v1_set_exclusive_zone(output->zwlr_layer_surface, -1);
    zwlr_layer_surface_v1_set_anchor(output->zwlr_layer_surface, ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP | ZWLR_LAYER_SURFACE_V1_ANCHOR_RIGHT | ZWLR_LAYER_SURFACE_V1_ANCHOR_BOTTOM | ZWLR_LAYER_SURFACE_V1_ANCHOR_LEFT);

    zwlr_layer_surface_v1_add_listener(output->zwlr_layer_surface, layer_surface_listener(), output);
    wl_surface_commit(output->wl_surface);
}
