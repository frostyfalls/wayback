#ifndef WAYBACK_H
#define WAYBACK_H

#include <stdbool.h>
#include <wayland-client.h>
#include <wlr-layer-shell-unstable-v1.h>

/* Physical output and properties */
struct wayback_output {
    uint32_t wl_name;
    struct wl_output *wl_output;
    char *make, *model;

    uint32_t width, height;
    int32_t scale;

    struct wl_surface *wl_surface;
    struct zwlr_layer_surface_v1 *zwlr_layer_surface;

    struct wayback_state *state;

    bool configured;
    bool needs_ack;
    uint32_t configure_serial;
    bool dirty;

    struct wl_list link;
};

/* Global application state */
struct wayback_state {
    struct wl_display *wl_display;
    struct wl_registry *wl_registry;
    struct wl_compositor *wl_compositor;
    struct wl_shm *wl_shm;
    struct wl_list outputs; /* struct wayback_output::link */
    struct zwlr_layer_shell_v1 *zwlr_layer_shell;
    bool running;
    int ret;
};

/* output.c:
 * Output management, mostly related to wayback_output structs. */
void destroy_wayback_output(struct wayback_output *output);

/* wayland.c:
 * Wayland global management, like registries and the compositor. */
bool init_wayland(struct wayback_state *state);
void finish_wayland(struct wayback_state *state);

#endif /* WAYBACK_H */
