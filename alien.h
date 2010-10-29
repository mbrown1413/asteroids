
#ifndef ASTEROIDS_ALIEN_H
#define ASTEROIDS_ALIEN_H

typedef struct {
    float x, y;
    float yaw, dyaw;
} Alien;

Alien* Alien_new();
void Alien_update(Alien* a, float screen_width);
void Alien_draw(Alien* a);
void Alien_free(Alien* alien);

#endif
