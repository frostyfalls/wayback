/* SPDX-License-Identifier: Unlicense */

#pragma once

#include "wayback.h"

void create_layer_surface(struct wayback_state *state,
                          struct wayback_output *output);
void render_surface(const struct wayback_output *output);
