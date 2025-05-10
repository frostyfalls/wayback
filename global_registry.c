#include <stdlib.h>
#include <string.h>
#include <wayland-client.h>

#include "registry.h"
#include "wayback.h"

static void handle_global(void *data, struct wl_registry *registry,
                          uint32_t name, const char *interface,
                          uint32_t version) {
    (void)version;
    struct wayback_state *state = data;

    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        state->wl_compositor =
            wl_registry_bind(registry, name, &wl_compositor_interface, 4);
    } else if (strcmp(interface, wl_shm_interface.name) == 0) {
        state->wl_shm = wl_registry_bind(registry, name, &wl_shm_interface, 1);
    } else if (strcmp(interface, wl_output_interface.name) == 0) {
        struct wayback_output *output =
            calloc(1, sizeof(struct wayback_output));
        output->wl_name = name;
        output->wl_output =
            wl_registry_bind(registry, name, &wl_output_interface, 3);
        wl_output_add_listener(output->wl_output, output_listener(), output);
        wl_list_insert(&state->outputs, &output->link);
    }
}

static void handle_global_remove(void *data, struct wl_registry *registry,
                                 uint32_t name) {
    (void)registry;

    struct wayback_state *state = data;

    struct wayback_output *output, *tmp;
    wl_list_for_each_safe(output, tmp, &state->outputs, link) {
        if (output->wl_name == name) {
            destroy_output(output);
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
