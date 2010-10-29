#ifndef ASTEROIDS_PARTICLES_H
#define ASTEROIDS_PARTICLES_H

#include "linked_list.h"

typedef struct {

    float x, y;
    float dx, dy;
    float ddx, ddy;

    float color[4];

} Particle;

void Explosion_new(
    float x, float y,
    float dx, float dy,
    int size, float color[],
    List* particles);
void Explosions_update(List* particles, float screen_width);
void Explosions_draw(List* particles);

#endif
