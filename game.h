
#ifndef ASTEROIDS_GAME_H
#define ASTEROIDS_GAME_H

#include "linked_list.h"
#include "alien.h"
#include "asteroid.h"
#include "bullet.h"
#include "player.h"

#define LEVEL_START_DELAY 50

typedef struct {
    List* aliens;
    List* asteroids;
    List* bullets;
    List* particles;
    Player* player;
    unsigned int level;
    unsigned int level_start_timer;

    float screen_width;
} Game;

Game* Game_new(unsigned int start_level);
void Game_draw(Game* g);
void Game_update(Game* g);
void Game_start_level(Game* g, unsigned int level);
void Game_free(Game* g);

#endif
