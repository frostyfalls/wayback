#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <wayland-client.h>

#include "registry.h"
#include "shm.h"
#include "wayback.h"

void create_layer_surface(struct wayback_output *output) {
    output->wl_surface = wl_compositor_create_surface(output->state->wl_compositor);

    struct wl_region *input_region = wl_compositor_create_region(output->state->wl_compositor);
    wl_surface_set_input_region(output->wl_surface, input_region);
    wl_region_destroy(input_region);

    output->zwlr_layer_surface = zwlr_layer_shell_v1_get_layer_surface(output->state->zwlr_layer_shell, output->wl_surface, output->wl_output, ZWLR_LAYER_SHELL_V1_LAYER_BOTTOM, "wallpaper");

    zwlr_layer_surface_v1_set_exclusive_zone(output->zwlr_layer_surface, -1);
    zwlr_layer_surface_v1_set_anchor(output->zwlr_layer_surface, ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP | ZWLR_LAYER_SURFACE_V1_ANCHOR_RIGHT | ZWLR_LAYER_SURFACE_V1_ANCHOR_BOTTOM | ZWLR_LAYER_SURFACE_V1_ANCHOR_LEFT);

    zwlr_layer_surface_v1_add_listener(output->zwlr_layer_surface, layer_surface_listener(), output);
    wl_surface_commit(output->wl_surface);

    /* XXX: Remove temporary hard-coded values and move into separate function.
     * This was just to get the very basic buffer setup working. */
    const int width = 1920, height = 1080;
    const int stride = width * 4;
    const int shm_pool_size = height * stride * 2;

    int fd = allocate_shm_file(shm_pool_size);
    uint8_t *pool_data = mmap(NULL, shm_pool_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    struct wl_shm_pool *pool = wl_shm_create_pool(output->state->wl_shm, fd, shm_pool_size);
    int index = 0;
    int offset = height * stride * index;
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, offset, width, height, stride, WL_SHM_FORMAT_XRGB8888);
    uint32_t *pixels = (uint32_t *)&pool_data[offset];
    memset(pixels, 0, width * height * 4);

    wl_surface_attach(output->wl_surface, buffer, 0, 0);
    wl_surface_damage(output->wl_surface, 0, 0, UINT32_MAX, UINT32_MAX);
}
