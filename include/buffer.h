/* SPDX-License-Identifier: Unlicense */

#pragma once

#include <stddef.h>
#include <wayland-client.h>

struct wayback_buffer {
    struct wl_buffer *wl_buffer;
    void *pool_data;
    size_t pool_size;
};

struct wayback_buffer *create_buffer(struct wl_shm *shm, uint32_t width,
                                     uint32_t height);
void destroy_buffer(struct wayback_buffer *buffer);
