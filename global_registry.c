/* SPDX-License-Identifier: Unlicense */

#include <string.h>
#include <wayland-client.h>
#include <wlr-layer-shell-unstable-v1.h>

#include "wayback.h"

/* Interface versions to handle */
#define WL_COMPOSITOR_VERSION 4
#define WL_OUTPUT_VERSION 4
#define WL_SHM_VERSION 1
#define ZWLR_LAYER_SHELL_VERSION 2

static void registry_global(void *data, struct wl_registry *registry,
                            uint32_t name, const char *interface,
                            uint32_t version) {
    (void)version;
    struct wayback_state *state = data;

    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        state->wl_compositor = wl_registry_bind(
            registry, name, &wl_compositor_interface, WL_COMPOSITOR_VERSION);
    }

    else if (strcmp(interface, wl_output_interface.name) == 0) {
        struct wayback_output *output =
            calloc(1, sizeof(struct wayback_output));
        output->wl_name = name;
        output->wl_shm = state->wl_shm;
        output->wl_output = wl_registry_bind(
            registry, name, &wl_output_interface, WL_OUTPUT_VERSION);
        tll_push_back(state->outputs, output);
        wl_output_add_listener(output->wl_output, output_listener(), output);
        create_layer_surface(state, output);
    }

    else if (strcmp(interface, wl_shm_interface.name) == 0) {
        state->wl_shm =
            wl_registry_bind(registry, name, &wl_shm_interface, WL_SHM_VERSION);

    }

    else if (strcmp(interface, zwlr_layer_shell_v1_interface.name) == 0) {
        state->zwlr_layer_shell =
            wl_registry_bind(registry, name, &zwlr_layer_shell_v1_interface,
                             ZWLR_LAYER_SHELL_VERSION);
    }
}

static void registry_global_remove(void *data, struct wl_registry *registry,
                                   uint32_t name) {
    (void)registry;
    struct wayback_state *state = data;

    tll_foreach(state->outputs, it) {
        if (it->item->wl_name == name) {
            destroy_output(it->item);
            break;
        }
    }
}

static const struct wl_registry_listener listener = {
    .global = registry_global,
    .global_remove = registry_global_remove,
};

const struct wl_registry_listener *registry_listener(void) {
    return &listener;
}
