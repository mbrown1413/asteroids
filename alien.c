
#include <stdio.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "alien.h"
#include "explosion.h"
#include "game.h"

#define PI 3.14159265358979

float alien_material[] = {0.2, 0.0, 0.0};

/**
 * Alien_new
 * Creates a new alien.
 */
Alien* Alien_new(bool large, float screen_width) {
    Alien* a = (Alien*) calloc(sizeof(Alien), 1);
    if (!a) {
        printf("Could not allocate memory!\n");
        printf("    In Alien_new()\n");
        exit(1);
    }
    a->x = screen_width/2 + 5;
    a->y = 8;
    a->dx = 0;
    a->dy = 0;
    a->yaw = 0;
    a->dyaw = 3;
    a->large = large;
    a->weapon_cooldown = 150;
    return a;
}

/**
 * Alien_update
 * Updates the given alien.
 */
void Alien_update(Alien* a, Game* game) {

    float dx = 0;
    float dy = 0;
    List_start_iteration(game->asteroids);
    Asteroid* asteroid;
    while ((asteroid = (Asteroid*) List_next(game->asteroids)))
    {
        float exponential_factor = 3.0/6;
        float constant_factor = 15;
        float distance = sqrt(
            (asteroid->x - a->x) * (asteroid->x - a->x) +
            (asteroid->y - a->y) * (asteroid->y - a->y)
        );
        dx += pow(exponential_factor, distance-3.5) * (a->x - asteroid->x) * constant_factor;
        dy += pow(exponential_factor, distance-3.5) * (a->y - asteroid->y) * constant_factor;
    }
    float player_factor = 0.02;
    Player* player = game->player;
    dx += (game->player->x - a->x) * player_factor;
    dy += (game->player->y - a->y) * player_factor;
    float player_distance = sqrt(
        (player->x - a->x) * (player->x - a->x) +
        (player->y - a->y) * (player->y - a->y)
    );
    float constant_factor = 0.5;
    dx += pow(5.0/6, player_distance) * (a->x - player->x) * constant_factor;
    dy += pow(5.0/6, player_distance) * (a->y - player->y) * constant_factor;

    float max_velocity_component = 0.01;
    if (dx > max_velocity_component) {
        dx = max_velocity_component;
    }
    if (dy > max_velocity_component) {
        dy = max_velocity_component;
    }

    a->x += dx;
    a->y += dy;
    a->yaw += a->dyaw;

    float screen_width = game->screen_width;
    float offscreen_distance = 10;
    if (a->x > screen_width/2 + offscreen_distance) {
        a->x = -screen_width/2 - offscreen_distance;
    } else if (a->x < -screen_width/2 - offscreen_distance) {
        a->x = screen_width/2 + offscreen_distance;
    }
    if (a->y > screen_width/2 + offscreen_distance) {
        a->y = -screen_width/2 - offscreen_distance;
    } else if (a->y < -screen_width/2 - offscreen_distance) {
        a->y = screen_width/2 + offscreen_distance;
    }

    if (a->weapon_cooldown > 0) {
        a->weapon_cooldown--;
    } else if (a->x < screen_width/2 &&
               a->x > -screen_width/2 &&
               a->y < screen_width/2 &&
               a->y > -screen_width/2)
    {

        // Fire!
        a->weapon_cooldown = 25;
        float player_direction_x = player->x - a->x;
        float player_direction_y = player->y - a->y;
        Bullet* b = Bullet_new(
            a->x,
            a->y,
            BULLET_VELOCITY * player_direction_x / player_distance + a->dx,
            BULLET_VELOCITY * player_direction_y / player_distance + a->dy,
            BULLET_LIFETIME,
            1, 0, 0,
            (void*) a
        );
        List_append(game->bullets, b);
        
    }
}

/**
 * Alien_update_list
 * Runs Alien_update() for each alien.
 */
void Alien_update_list(List* aliens, Game* game) {
    List_start_iteration(aliens);
    Alien* alien;
    while ((alien = (Alien*) List_next(aliens)))
    {
        Alien_update(alien, game);
    }
}

/**
 * Alien_draw
 * Draws the given alien.
 */
void Alien_draw(Alien* a) {

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, alien_material);

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();

    // Transforms
    glTranslatef(a->x, a->y, 0.0);
    glRotatef(-45, 1, 0, 0);
    glRotatef(a->yaw, 0, 0, 1);
    glScalef(0.5, 0.5, 0.5);

    glutSolidTorus(2, 5, 30, 5);
    glutSolidTorus(0.5, 2.5, 30, 5);
    glutSolidTorus(1, .1, 30, 5);

    glPopMatrix();
}

/**
 * Alien_draw_list
 * Runs Alien_draw() for each alien.
 */
void Alien_draw_list(List* aliens) {
    List_start_iteration(aliens);
    Alien* alien;
    while ((alien = (Alien*) List_next(aliens)))
    {
        Alien_draw(alien);
    }
}

/**
 * Alien_die
 * Kills the given alien
 */
void Alien_die(Alien* alien, List* particles)
{
    float alien_material[] = {1.0, 0.0, 0.0};
    Explosion_new(alien->x, alien->y, 0,
        0, 3, alien_material, particles);
}

/**
 * Alien_free
 * Frees the memory for the given Alien.
 */
void Alien_free(Alien* alien) {
    free(alien);
}
