
#include <stdio.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "alien.h"
#include "explosion.h"
#include "game.h"

#define PI 3.14159265358979

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
    a->x = 8;
    a->y = 8;
    a->dx = 0;
    a->dy = 0;
    a->yaw = 0;
    a->dyaw = 3;
    a->large = large;
    a->weapon_cooldown = 50;
    return a;
}

/**
 * Alien_update
 * Updates the given alien.
 */
void Alien_update(Alien* a, Game* game) {
    a->yaw += a->dyaw;
    if (a->weapon_cooldown > 0) {
        a->weapon_cooldown++;
    } else {

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

    float alien_material[] = {0.2, 0.0, 0.0};
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
        0, 4, alien_material, particles);
}

/**
 * Alien_free
 * Frees the memory for the given Alien.
 */
void Alien_free(Alien* alien) {
    free(alien);
}
