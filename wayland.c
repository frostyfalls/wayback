#include <stdio.h>
#include <string.h>
#include <wayland-client.h>

#include "registry.h"
#include "wayback.h"

bool init_wayland(struct wayback_state *state) {
    state->wl_display = wl_display_connect(NULL);
    if (state->wl_display == NULL) {
        fprintf(stderr, "err: Failed to connect to Wayland display.\n");
        return false;
    }
    state->wl_registry = wl_display_get_registry(state->wl_display);
    if (state->wl_registry == NULL) {
        fprintf(stderr, "err: Failed to get display registry.\n");
        return false;
    }

    wl_list_init(&state->outputs);

    wl_registry_add_listener(state->wl_registry, registry_listener(), state);
    wl_display_roundtrip(state->wl_display);

    if (state->wl_compositor == NULL) {
        fprintf(stderr, "err: Wayland compositor not found.\n");
        return false;
    }
    if (state->wl_shm == NULL) {
        fprintf(stderr, "err: Wayland shared memory not found.\n");
        return false;
    }
    if (state->zwlr_layer_shell == NULL) {
        fprintf(stderr, "err: Wayland layer shell not found.\n");
        return false;
    }

    return true;
}

void finish_wayland(struct wayback_state *state) {
    struct wayback_output *output, *tmp;
    wl_list_for_each_safe(output, tmp, &state->outputs, link) {
        destroy_wayback_output(output);
    }
    if (state->wl_compositor != NULL) {
        wl_compositor_destroy(state->wl_compositor);
        state->wl_compositor = NULL;
    }
    if (state->wl_shm != NULL) {
        wl_shm_destroy(state->wl_shm);
        state->wl_shm = NULL;
    }
    if (state->wl_registry != NULL) {
        wl_registry_destroy(state->wl_registry);
        state->wl_registry = NULL;
    }
    if (state->wl_display != NULL) {
        wl_display_disconnect(state->wl_display);
        state->wl_display = NULL;
    }
}
