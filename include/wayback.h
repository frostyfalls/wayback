/* SPDX-License-Identifier: Unlicense */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <tllist.h>

struct wayback_output {
    uint32_t wl_name;
    struct wl_output *wl_output;
    char *make, *model, *name;

    uint32_t width, height;
    int32_t scale;

    struct wl_surface *wl_surface;
    struct zwlr_layer_surface_v1 *zwlr_layer_surface;

    /* Globals, do not destroy upon closing output */
    struct wl_shm *wl_shm;

    bool configured;
};

struct wayback_state {
    struct wl_display *wl_display;
    struct wl_registry *wl_registry;
    struct wl_compositor *wl_compositor;
    struct wl_shm *wl_shm;
    struct zwlr_layer_shell_v1 *zwlr_layer_shell;

    tll(struct wayback_output *) outputs;

    bool running;
    int ret;
};
