#include <stdio.h>
#include <wayland-client.h>

#include "wayback.h"

int main(int argc, char *argv[]) {
    (void)argc;

    printf("%s v" WAYBACK_VERSION "\n", argv[0]);

    struct wayback_state state = {
        .running = true,
        .ret = EXIT_SUCCESS,
    };

    if (!init_wayland(&state)) {
        state.running = false;
        state.ret = EXIT_FAILURE;
    }

    while (state.running && wl_display_dispatch(state.wl_display) != -1)
        ;

    finish_wayland(&state);
    return state.ret;
}
