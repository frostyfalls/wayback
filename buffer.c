/* SPDX-License-Identifier: Unlicense */

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <wayland-client.h>

#include "wayback.h"

static int32_t create_shm_file(void) {
    uint8_t retries = 8;
    do {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        pid_t pid = getpid();
        char name[32];
        snprintf(name, sizeof(name), "/wayback-%x-%x", pid,
                 (uint32_t)ts.tv_nsec);

        int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
        if (fd >= 0) {
            shm_unlink(name);
            return fd;
        }
        retries--;
    } while (retries > 0 && errno == EEXIST);
    return -1;
}

struct wayback_buffer *create_buffer(struct wl_shm *shm, uint32_t width,
                                     uint32_t height) {
    struct wayback_buffer *buffer = calloc(1, sizeof(struct wayback_buffer));
    uint32_t stride = width * 4;
    uint32_t size = height * stride;

    int32_t fd = create_shm_file();
    if (fd == -1 || ftruncate(fd, size) < 0) {
        close(fd);
        return false;
    }

    void *data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    struct wl_shm_pool *pool = wl_shm_create_pool(shm, fd, size);
    buffer->wl_buffer = wl_shm_pool_create_buffer(
        pool, 0, width, height, stride, WL_SHM_FORMAT_XRGB8888);

    wl_shm_pool_destroy(pool);
    pool = NULL;
    close(fd);
    fd = -1;

    buffer->pool_size = size;
    buffer->pool_data = data;

    return buffer;
}

void destroy_buffer(struct wayback_buffer *buffer) {
    if (buffer->wl_buffer != NULL) {
        wl_buffer_destroy(buffer->wl_buffer);
        buffer->wl_buffer = NULL;
    }
    if (buffer->pool_data != NULL) {
        munmap(buffer->pool_data, buffer->pool_size);
        buffer->pool_data = NULL;
    }
}
