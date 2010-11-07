
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979

#include "linked_list.h"
#include "alien.h"
#include "asteroid.h"
#include "bullet.h"
#include "explosion.h"
#include "player.h"
#include "interactions.h"

float distance(float x1, float y1, float x2, float y2)
{
    return sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
}

void check_collisions(Game* game) {

    List* aliens = game->aliens;
    List* asteroids = game->asteroids;
    List* bullets = game->bullets;
    List* particles = game->particles;
    Player* player = game->player;
    float screen_width = game->screen_width;

    Asteroid* asteroid;
    Alien* alien;
    Bullet* bullet;

    // Bullet Collisions
    List_start_iteration(bullets);
    while ((bullet = (Bullet*) List_next(bullets)))
    {

        // Asteroids <-> Bullets
        List_start_iteration(asteroids);
        while ((asteroid = (Asteroid*) List_next(asteroids)))
        {
            if (check_collision_asteroid_bullet(game, asteroid, bullet)) {
                continue;
            }
        }

        // Aliens <-> Bullets
        List_start_iteration(aliens);
        while ((alien = (Alien*) List_next(aliens)))
        {
            check_collision_alien_bullet(game, alien, bullet);
        }

        // Players <-> Bullets
        check_collision_bullet_player(game, bullet, player);

    }

    // Asteroid Collisions
    List_start_iteration(asteroids);
    while ((asteroid = (Asteroid*) List_next(asteroids)))
    {

        // Players <-> Asteroids
        check_collision_asteroid_player(game, asteroid, player);

        // Aliens <-> Asteroids
        List_start_iteration(aliens);
        while ((alien = (Alien*) List_next(aliens)))
        {
            check_collision_asteroid_alien(game, asteroid, alien);
        }

    }

    // Aliens <-> Players
    List_start_iteration(aliens);
    while ((alien = (Alien*) List_next(aliens)))
    {
        check_collision_alien_player(game, alien, player);
    }

}

bool check_collision_asteroid_bullet(Game* game, Asteroid* asteroid, Bullet* bullet)
{
    for (int x_offset=-1; x_offset<=1; x_offset++) {
        for (int y_offset=-1; y_offset<=1; y_offset++) {
            float i = x_offset*game->screen_width;
            float j = y_offset*game->screen_width;

            float d = distance(i+bullet->x, j+bullet->y, asteroid->x, asteroid->y);
            if (d < asteroid->radius)
            {

                bullet->frames_to_live = 0;
                game->player->score += 20*asteroid->radius;
                Asteroid* new_asteroid = Asteroid_split(asteroid, game->particles, bullet->dx, bullet->dy);
                if (new_asteroid == NULL) {
                    Asteroid_free(asteroid);
                    List_remove_current(game->asteroids);
                } else {
                    List_append(game->asteroids, (void*) new_asteroid);
                }

                return true;

            }

        }
    }

    return false;
}

bool check_collision_asteroid_player(Game* game, Asteroid* asteroid, Player* player)
{
    if (!player->dead) {

        for (int x_offset=-1; x_offset<=1; x_offset++) {
            for (int y_offset=-1; y_offset<=1; y_offset++) {
                float i = x_offset*game->screen_width;
                float j = y_offset*game->screen_width;

                float point_ax = player->x + 2.0*cos((PI/180)*player->yaw); // Top point of the player's ship
                float point_ay = player->y + 2.0*sin((PI/180)*player->yaw);
                float point_bx = player->x + 0.25*sin((PI/180)*player->yaw); // Right point
                float point_by = player->y + -0.25*cos((PI/180)*player->yaw);
                float point_cx = player->x + -0.25*sin((PI/180)*player->yaw); // Left point
                float point_cy = player->y + 0.25*cos((PI/180)*player->yaw);
                float d0 = distance(point_ax, point_ay, i+asteroid->x, j+asteroid->y);
                float d1 = distance(point_bx, point_by, i+asteroid->x, j+asteroid->y);
                float d2 = distance(point_cx, point_cy, i+asteroid->x, j+asteroid->y);
                if (d0 < asteroid->radius ||
                    d1 < asteroid->radius ||
                    d2 < asteroid->radius)
                {

                    Player_die(player, game->particles);
                    player->score += 20*asteroid->radius;
                    Asteroid* new_asteroid = Asteroid_split(asteroid, game->particles, player->dx, player->dy);
                    if (new_asteroid == NULL) {
                        Asteroid_free(asteroid);
                        List_remove_current(game->asteroids);
                    } else {
                        List_append(game->asteroids, (void*) new_asteroid);
                    }

                    return true;
                }
            }
        }
    }
    return false;
}

bool check_collision_asteroid_alien(Game* game, Asteroid* asteroid, Alien* alien)
{
    float d = distance(asteroid->x, asteroid->y, alien->x, alien->y);
    if (d <= 3+asteroid->radius)
    {

        game->player->score += 200;
        Alien_die(alien, game->particles);
        Alien_free(alien);
        List_remove_current(game->aliens);

        Asteroid* new_asteroid = Asteroid_split(asteroid, game->particles, alien->dx, alien->dy);
        if (new_asteroid == NULL) {
            Asteroid_free(asteroid);
            List_remove_current(game->asteroids);
        } else {
            List_append(game->asteroids, (void*) new_asteroid);
        }

    }
}

bool check_collision_alien_player(Game* game, Alien* alien, Player* player)
{
    if (!player->dead) {

        for (int x_offset=-1; x_offset<=1; x_offset++) {
            for (int y_offset=-1; y_offset<=1; y_offset++) {
                float i = x_offset*game->screen_width;
                float j = y_offset*game->screen_width;

                float point_ax = player->x + 2.0*cos((PI/180)*player->yaw); // Top point of the player's ship
                float point_ay = player->y + 2.0*sin((PI/180)*player->yaw);
                float point_bx = player->x + 0.25*sin((PI/180)*player->yaw); // Right point
                float point_by = player->y + -0.25*cos((PI/180)*player->yaw);
                float point_cx = player->x + -0.25*sin((PI/180)*player->yaw); // Left point
                float point_cy = player->y + 0.25*cos((PI/180)*player->yaw);
                float d0 = distance(point_ax, point_ay, i+alien->x, j+alien->y);
                float d1 = distance(point_bx, point_by, i+alien->x, j+alien->y);
                float d2 = distance(point_cx, point_cy, i+alien->x, j+alien->y);
                if (d0 < 3.1 ||
                    d1 < 3.1 ||
                    d2 < 3.1)
                {

                    Player_die(player, game->particles);
                    player->score += 200;
                    Alien_die(alien, game->particles);
                    Alien_free(alien);
                    List_remove_current(game->aliens);

                    return true;
                }
            }
        }
    }
    return false;
}

bool check_collision_alien_bullet(Game* game, Alien* alien, Bullet* bullet)
{
    float d = distance(alien->x, alien->y, bullet->x, bullet->y);
    if (d < 3.1) {
        Alien_die(alien, game->particles);
        game->player->score += 200;
        List_remove_current(game->aliens);
        bullet->frames_to_live = 0;
    }
}

bool check_collision_bullet_player(Game* game, Bullet* bullet, Player* player)
{
}
