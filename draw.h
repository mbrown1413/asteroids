#ifndef ASTEROIDS_DRAW_H
#define ASTEROIDS_DRAW_H

#include "game.h"

void reshape(int w, int h);
void draw_all(Game* game);
void draw_objects(Game* game);
void draw_hud(Game* game);

#endif
