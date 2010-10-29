
#ifndef ASTEROIDS_ASTEROID_H
#define ASTEROIDS_ASTEROID_H

#include "linked_list.h"

typedef struct {

    // The vertices in the same order as in asteroid_vertices.
    // The vertices will be the same as asteroid_vertices with a little
    // variation to make this asteroid unique.
    float vertices[18][3];

    // Position and orientation and derivatives
    float x, y;
    float dx, dy;
    float rot_x, rot_y, rot_z;
    float rot_dx, rot_dy, rot_dz;

    float radius;

    float variance;

} Asteroid;

Asteroid* Asteroid_new(
        float size,
        float x, float y,
        float dx, float dy,
        float rot_dx, float rot_dy, float rot_dz
    );
Asteroid* Asteroid_new_random(float radius, float screen_width);
Asteroid* Asteroid_split(Asteroid* a, List* particles);
void Asteroid_update(Asteroid* a, float screen_width);
void Asteroid_update_list(List* asteroids, float screen_width);
void Asteroid_draw(Asteroid* a);
void Asteroid_draw_list(List* asteroid_list);
void Asteroid_free(Asteroid* a);

#endif
