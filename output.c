#include <stdlib.h>

#include "wayback.h"

void destroy_wayback_output(struct wayback_output *output) {
    if (output == NULL) {
        return;
    }

    wl_list_remove(&output->link);
    wl_output_destroy(output->wl_output);
    free(output->make);
    free(output->model);
    free(output);
}
