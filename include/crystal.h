#ifndef ASTEROID_CRYSTAL_H
#define ASTEROID_CRYSTAL_H

#include <stdbool.h>

#include "linked_list.h"

typedef struct {
    float x, y;
    float dx, dy;
    float spin_axis[3];
    float spin_rate;
    float spin;

    unsigned int frames_to_live;
} Crystal;

Crystal* Crystal_new(float x, float y, float dx, float dy);
void Crystal_draw(Crystal* c);
void Crystal_draw_list(List* crystals);
bool Crystal_update(Crystal* c, float screen_width);
void Crystal_update_list(List* crystals, float screen_width);
void Crystal_explode(Crystal* c, List* particles);
void Crystal_free(Crystal* c);

#endif
