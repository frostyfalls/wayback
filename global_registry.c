#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wayland-client.h>
#include <wlr-layer-shell-unstable-v1.h>

#include "registry.h"
#include "wayback.h"

/* Versions of each interface handled */
#define WL_COMPOSITOR_VERSION 3
#define WL_OUTPUT_VERSION 3
#define WL_SHM_VERSION 1
#define ZWLR_LAYER_SHELL_VERSION 2

static void handle_global(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version) {
    (void)version;
    struct wayback_state *state = data;

    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        state->wl_compositor = wl_registry_bind(registry, name, &wl_compositor_interface, WL_COMPOSITOR_VERSION);
    } else if (strcmp(interface, wl_output_interface.name) == 0) {
        struct wayback_output *output = calloc(1, sizeof(struct wayback_output));
        output->wl_name = name;
        output->wl_output = wl_registry_bind(registry, name, &wl_output_interface, WL_OUTPUT_VERSION);
        output->state = state;
        output->needs_ack = true;
        wl_output_add_listener(output->wl_output, output_listener(), output);
        wl_list_insert(&state->outputs, &output->link);
    } else if (strcmp(interface, wl_shm_interface.name) == 0) {
        state->wl_shm = wl_registry_bind(registry, name, &wl_shm_interface, WL_SHM_VERSION);
    } else if (strcmp(interface, zwlr_layer_shell_v1_interface.name) == 0) {
        state->zwlr_layer_shell = wl_registry_bind(registry, name, &zwlr_layer_shell_v1_interface, ZWLR_LAYER_SHELL_VERSION);
    }
}

static void handle_global_remove(void *data, struct wl_registry *registry, uint32_t name) {
    (void)registry;
    struct wayback_state *state = data;

    struct wayback_output *output, *tmp;
    wl_list_for_each_safe(output, tmp, &state->outputs, link) {
        if (output->wl_name == name) {
            destroy_wayback_output(output);
            break;
        }
    }
}

static const struct wl_registry_listener listener = {
    .global = handle_global,
    .global_remove = handle_global_remove,
};

const struct wl_registry_listener *registry_listener(void) {
    return &listener;
}
