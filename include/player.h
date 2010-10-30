
#ifndef ASTEROIDS_PLAYER_H
#define ASTEROIDS_PLAYER_H

#include <stdbool.h>

#include "bullet.h"

//#define CAP_VELOCITY
#define THRUST_ACCELERATION 0.02
#define TURN_RATE 5
#define MAXIMUM_VELOCITY 1
#define BULLET_LIFETIME 30
#define BULLET_VELOCITY 1.5

typedef struct {

    // Color represented by RGBA
    float mat[4];

    // Position, orientation and movement
    float x, y;
    float dx, dy;
    float yaw; // yaw=0 is pointing to the left

    // dyaw will be -1, 0, or 1 depending on which direction the player is
    // turning the ship.
    float dyaw;

    // thrust is 1 if the ship is being propelled forward.  0 otherwise.
    float thrust;

    // 0 is completely cool, higher numbers are hotter
    unsigned int weapon_cooldown;

    // True if the player is alive, false if dead.
    // This is false for a short period after the player has died, even if the
    // player has more extra_lives.
    bool dead;

    // Number of extra lives left.  Once this reaches zero, the player will not
    // be able to spawn after the next death.
    unsigned int extra_lives;

    int spawn_timer;

} Player;

Player* Player_new(float x, float y, float r, float g, float b);
void Player_draw(Player* p);
Bullet* Player_fire(Player* p);
void Player_update(Player* p, float screen_width);
void Player_spawn(Player* p, float x, float y);
void Player_die(Player* p, List* particles);

#endif
