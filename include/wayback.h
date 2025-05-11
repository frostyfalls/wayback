#ifndef WAYBACK_H
#define WAYBACK_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tllist.h>

struct wayback_buffer {
    struct wl_buffer *wl_buffer;
    void *pool_data;
    size_t pool_size;
};

struct wayback_output {
    uint32_t wl_name;
    struct wl_output *wl_output;
    char *make, *model;
    char *name;

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

/* buffer.c */
bool create_buffer(struct wayback_buffer *buffer, struct wl_shm *shm,
                   uint32_t width, uint32_t height, uint32_t format);
void destroy_buffer(struct wayback_buffer *buffer);

/* layer_surface.c */
void create_layer_surface(struct wayback_state *state,
                          struct wayback_output *output);
void render_surface(const struct wayback_output *output);

/* output.c */
void destroy_output_layer(struct wayback_output *output);
void destroy_output(struct wayback_output *output);

/* wayland.c */
bool init_wayland(struct wayback_state *state);
void finish_wayland(struct wayback_state *state);

#endif /* WAYBACK_H */
