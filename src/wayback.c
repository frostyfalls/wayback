/* SPDX-License-Identifier: Unlicense */

#include <stdio.h>
#include <wayland-client.h>
#include <wlr-layer-shell-unstable-v1.h>

#include "output.h"
#include "registry_listener.h"
#include "wayback.h"

bool init_wayland(struct wayback_state *state) {
    state->wl_display = wl_display_connect(NULL);
    if (state->wl_display == NULL) {
        fprintf(stderr, "failed to connect to wayland display\n");
        return false;
    }
    state->wl_registry = wl_display_get_registry(state->wl_display);
    if (state->wl_registry == NULL) {
        fprintf(stderr, "failed to get display registry\n");
        return false;
    }

    wl_registry_add_listener(state->wl_registry, registry_listener(), state);
    wl_display_roundtrip(state->wl_display);

    if (state->wl_compositor == NULL || state->wl_shm == NULL ||
        state->zwlr_layer_shell == NULL) {
        fprintf(stderr, "unsupported compositor\n");
        return false;
    }

    return true;
}

void finish_wayland(struct wayback_state *state) {
    tll_foreach(state->outputs, it) {
        destroy_output(it->item);
    }
    tll_free(state->outputs);
    if (state->zwlr_layer_shell != NULL) {
        zwlr_layer_shell_v1_destroy(state->zwlr_layer_shell);
        state->zwlr_layer_shell = NULL;
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

int main(int argc, char *argv[]) {
    (void)argc;

    printf("%s v" WAYBACK_VERSION "\n", argv[0]);

    struct wayback_state state = {.running = false, .ret = EXIT_FAILURE};
    if (init_wayland(&state)) {
        state.running = true;
        state.ret = EXIT_SUCCESS;
    }

    while (state.running && wl_display_dispatch(state.wl_display) != -1)
        ;

    finish_wayland(&state);
    return state.ret;
}
