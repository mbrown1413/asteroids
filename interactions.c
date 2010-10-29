
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

void check_collisions(
    List* aliens,
    List* asteroids,
    List* bullets,
    List* particles,
    Player* player,
    float screen_width
) {
    Bullet* bullet;
    Asteroid* asteroid;
    //TODO: Split this eye sore into more functions

    for (int x_offset=-1; x_offset<=1; x_offset++) {
        for (int y_offset=-1; y_offset<=1; y_offset++) {
            float i = x_offset*screen_width;
            float j = y_offset*screen_width;


            // Asteroid Collisions
            List_start_iteration(asteroids);
            while ((asteroid = (Asteroid*) List_next(asteroids)))
            {

                // Asteroids <-> bullets
                List_start_iteration(bullets);
                while ((bullet = (Bullet*) List_next(bullets)))
                {

                    float d = distance(i+bullet->x, j+bullet->y, asteroid->x, asteroid->y);
                    if (d < asteroid->radius)
                    {

                        Bullet_free(bullet);
                        List_remove_current(bullets);

                        Asteroid* new_asteroid = Asteroid_split(asteroid, particles);
                        if (new_asteroid == NULL) {
                            Asteroid_free(asteroid);
                            List_remove_current(asteroids);
                        } else {
                            List_append(asteroids, (void*) new_asteroid);
                        }
                        continue;

                    }
                }

                // Asteroids <-> Players
                if (!player->dead) {

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
                        Explosion_new(player->x, player->y, player->dx,
                            player->dy, 1, player->mat, particles);
                        player->dead = true;
                        Asteroid* new_asteroid = Asteroid_split(asteroid, particles);
                        if (new_asteroid == NULL) {
                            Asteroid_free(asteroid);
                            List_remove_current(asteroids);
                        } else {
                            List_append(asteroids, (void*) new_asteroid);
                        }
                    }
                }

            }
        }
    }

}
