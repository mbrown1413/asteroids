
#ifndef ASTEROID_COLLISIONS_H
#define ASTEROID_COLLISIONS_H

#include "alien.h"
#include "asteroid.h"
#include "bullet.h"
#include "crystal.h"
#include "explosion.h"
#include "player.h"
#include "game.h"

void check_collisions(Game* game);

bool check_collision_asteroid_bullet(Game* game, Asteroid* asteroid, Bullet* bullet);
bool check_collision_asteroid_player(Game* game, Asteroid* asteroid, Player* player);
bool check_collision_asteroid_alien(Game* game, Asteroid* asteroid, Alien* alien);

bool check_collision_alien_player(Game* game, Alien* alien, Player* player);
bool check_collision_alien_bullet(Game* game, Alien* alien, Bullet* bullet);

bool check_collision_bullet_player(Game* game, Bullet* bullet, Player* player);
bool check_collision_bullet_player(Game* game, Bullet* bullet, Player* player);

bool check_collision_bullet_crystal(Game* game, Bullet* bullet, Crystal* crystal);
bool check_collisions_crystal_player(Game* game, Crystal* crystal, Player* player);
bool check_collisions_asteroid_crystal(Game* game, Asteroid* asteroid, Crystal* crystal);

#endif
