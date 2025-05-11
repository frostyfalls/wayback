#include <wayland-client.h>
#include <wlr-layer-shell-unstable-v1.h>

#include "registry.h"
#include "wayback.h"

/* Description to use for the layer shell(s) we create */
#define LAYER_SURFACE_DESCRIPTION "wallpaper"

void create_layer_surface(struct wayback_state *state,
                          struct wayback_output *output) {
    output->wl_surface = wl_compositor_create_surface(state->wl_compositor);

    struct wl_region *input_region =
        wl_compositor_create_region(state->wl_compositor);
    wl_surface_set_input_region(output->wl_surface, input_region);
    wl_region_destroy(input_region);

    struct wl_region *opaque_region =
        wl_compositor_create_region(state->wl_compositor);
    wl_surface_set_opaque_region(output->wl_surface, opaque_region);
    wl_region_destroy(opaque_region);

    output->zwlr_layer_surface = zwlr_layer_shell_v1_get_layer_surface(
        state->zwlr_layer_shell, output->wl_surface, output->wl_output,
        ZWLR_LAYER_SHELL_V1_LAYER_BACKGROUND, LAYER_SURFACE_DESCRIPTION);

    zwlr_layer_surface_v1_set_size(output->zwlr_layer_surface, 0, 0);
    zwlr_layer_surface_v1_set_exclusive_zone(output->zwlr_layer_surface, -1);
    zwlr_layer_surface_v1_set_anchor(output->zwlr_layer_surface,
                                     ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP |
                                         ZWLR_LAYER_SURFACE_V1_ANCHOR_RIGHT |
                                         ZWLR_LAYER_SURFACE_V1_ANCHOR_BOTTOM |
                                         ZWLR_LAYER_SURFACE_V1_ANCHOR_LEFT);

    zwlr_layer_surface_v1_add_listener(output->zwlr_layer_surface,
                                       zwlr_layer_surface_listener(), output);
    wl_surface_commit(output->wl_surface);
}

void render_surface(const struct wayback_output *output) {
    struct wayback_buffer *buffer =
        create_buffer(output->wl_shm, output->width * output->scale,
                      output->height * output->scale);

    uint32_t *pool_data_uint = (uint32_t *)buffer->pool_data;
    uint32_t *pixels = &pool_data_uint[0];
    for (uint32_t y = 0; y < output->height; y++) {
        for (uint32_t x = 0; x < output->width; x++) {
            pixels[y * output->width + x] =
                ((x + y / 8 * 8) % 16 < 8) ? 0xFF000000 : 0xFFFFFFFF;
        }
    }

    wl_surface_set_buffer_scale(output->wl_surface, output->scale);
    wl_surface_attach(output->wl_surface, buffer->wl_buffer, 0, 0);
    wl_surface_damage_buffer(output->wl_surface, 0, 0,
                             output->width * output->scale,
                             output->height * output->scale);
    wl_surface_commit(output->wl_surface);

    destroy_buffer(buffer);
}
