#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "wayback.h"

static const char *wayback_usage =
    "usage: %s [-hV]\n"
    "  -h, --help     show help message and quit\n"
    "  -V, --version  show version information and quit\n";

static const struct option long_options[] = {
    {"help",    no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'V'},
    {NULL,      no_argument, NULL, 0  },
};

void parse_command_line(int argc, char *argv[]) {
    int c;
    opterr = 0;
    while ((c = getopt_long(argc, argv, "hVc:", long_options, NULL)) != -1) {
        switch (c) {
        case 'h':
            fprintf(stderr, wayback_usage, argv[0]);
            exit(EXIT_SUCCESS);
        case 'V':
            fprintf(stderr, "%s v%s\n", argv[0], WAYBACK_VERSION);
            exit(EXIT_SUCCESS);
        case ':':
            fprintf(stderr, "%s: option requires an argument -- '%c'\n", argv[0], optopt);
            fprintf(stderr, wayback_usage, argv[0]);
            exit(EXIT_FAILURE);
        case '?':
            fprintf(stderr, "%s: invalid option -- '%c'\n", argv[0], optopt);
            fprintf(stderr, wayback_usage, argv[0]);
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char *argv[]) {
    parse_command_line(argc, argv);

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
