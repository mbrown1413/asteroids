
#ifndef ASTEROIDS_ALIEN_H
#define ASTEROIDS_ALIEN_H

#include <stdbool.h>

#include "game.h"
#include "linked_list.h"

typedef struct {
    float x, y;
    float dx, dy;
    float yaw, dyaw;
    bool large;

    // 0 is completely cool, higher numbers are hotter
    unsigned int weapon_cooldown;

} Alien;

Alien* Alien_new(bool large, float screen_width);
void Alien_update(Alien* a, Game* game);
void Alien_update_list(List* aliens, Game* game);
void Alien_draw(Alien* a);
void Alien_draw_list(List* aliens);
void Alien_die(Alien* alien, List* particles);
void Alien_free(Alien* alien);

#endif
