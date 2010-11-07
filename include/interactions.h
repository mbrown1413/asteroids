
#ifndef ASTEROID_INTERACTIONS_H
#define ASTEROID_INTERACTIONS_H

void check_collisions(Game* game);

bool check_collision_asteroid_bullet(Game* game, Asteroid* asteroid, Bullet* bullet);
bool check_collision_asteroid_player(Game* game, Asteroid* asteroid, Player* player);
bool check_collision_asteroid_alien(Game* game, Asteroid* asteroid, Alien* alien);
bool check_collision_alien_player(Game* game, Alien* alien, Player* player);
bool check_collision_alien_bullet(Game* game, Alien* alien, Bullet* bullet);
bool check_collision_bullet_player(Game* game, Bullet* bullet, Player* player);

#endif
