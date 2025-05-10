#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "wayback.h"

static const char *wayback_usage =
    "usage: %s [-hV]\n"
    "  -h, --help     Show help message and quit\n"
    "  -V, --version  Show version information and quit\n";

static const struct option long_options[] = {
    {"help",    no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'V'},
    {0,         0,           0,    0  },
};

void parse_options(int argc, char *argv[]) {
    int c;
    while ((c = getopt_long(argc, argv, ":hV", long_options, NULL)) != -1) {
        switch (c) {
        case 'h':
            fprintf(stderr, wayback_usage, argv[0]);
            exit(EXIT_SUCCESS);
        case 'V':
            fprintf(stderr, "%s v%s\n", argv[0], WAYBACK_VERSION);
            exit(EXIT_SUCCESS);
        case ':':
            fprintf(stderr, "%s: option requires an argument -- '%c'\n",
                    argv[0], optopt);
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
    parse_options(argc, argv);

    struct wayback_state state = {
        .ret = EXIT_SUCCESS,
        .running = true,
    };
    if (!init_wayland(&state)) {
        finish_wayland(&state);
        return state.ret;
    }

    while (state.running && wl_display_dispatch(state.wl_display) != -1)
        ;

    return state.ret;
}
