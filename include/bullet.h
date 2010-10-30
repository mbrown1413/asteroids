
#ifndef ASTEROIDS_BULLET_H
#define ASTEROIDS_BULLET_H

#include <stdbool.h>

#include "linked_list.h"

typedef struct {

    //TODO: color

    // Position, orientation
    float x, y;
    float dx, dy;
    float color[3];

    // The number of frames until this bullet is destroyed.
    int frames_to_live;

} Bullet;

Bullet* Bullet_new(
    float x, float y,
    float dx, float dy,
    int frames_to_live,
    float r, float g, float b
);
void Bullet_draw(Bullet* b, float screen_width);
void Bullet_draw_list(List* bullets, float screen_width);
void Bullet_update_list(List* bullets, float screen_width);
bool Bullet_update(Bullet* b, float screen_width);
void Bullet_free(Bullet* bullet);

#endif
