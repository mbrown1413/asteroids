/**
 * explosion.c
 * A rudimentary particle explosion system.
 *
 * Since after an explosion is created, the particles that are created don't
 * need to be traced back to the explosion, we won't keep this information.
 * There is one master list of particles.  When explosions are created, the
 * resulting particles will be added to that list.
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "explosion.h"

/**
 * Explosion_new
 * Adds a number of particles proportional to the given size to the particle
 * list.  The particles will radiate outward from the given x,y position.
 */
void Explosion_new(
    float x, float y,
    float dx, float dy,
    int size, float color[],
    List* particles)
{

    int particle_number = size*sqrt(size)*15;
    for (int i=0; i<particle_number; i++) {

        Particle* p = (Particle*) calloc(sizeof(Particle), 1);
        if (!p) {
            printf("Could not allocate memory!\n");
            printf("    In Explosion_new()\n");
            exit(1);
        }

        // Movement
        p->x = x + (rand()%2000 / 1000.0 - 1) * size/2;
        p->y = y + (rand()%2000 / 1000.0 - 1) * size/2;
        p->dx = dx + (rand()%2000 / 1000.0 - 1)/10.0;
        p->dy = dy + (rand()%2000 / 1000.0 - 1)/10.0;
        p->ddx = (rand()%2000 / 1000.0 - 1)/500;
        p->ddy = (rand()%2000 / 1000.0 - 1)/500;

        // Color
        p->color[0] = color[0];
        p->color[1] = color[1];
        p->color[2] = color[2];
        p->color[3] = color[3];

        List_append(particles, p);

    }
}

/**
 * Explosions_update
 * Updates the movement and degradation of each particle.  To be called every
 * frame.
 */
void Explosions_update(List* particles, float screen_width)
{
    List_start_iteration(particles);
    Particle* p = NULL;
    List_start_iteration(particles);
    while ((p = (Particle*) List_next(particles)))
    {

        // Movement
        p->dx += p->ddx;
        p->dy += p->ddy;
        p->x += p->dx;
        p->y += p->dy;
        p->dx *= 0.99;
        p->dy *= 0.99;
        if (fabs(p->x) > screen_width/2) p->x = -p->x;
        if (fabs(p->y) > screen_width/2) p->y = -p->y;

        // Color degrade
        p->color[0] -= 0.01;
        p->color[1] -= 0.01;
        p->color[2] -= 0.01;
        p->color[0] *= 0.99;
        p->color[1] *= 0.99;
        p->color[2] *= 0.99;
        if (p->color[0] <= 0.0) p->color[0] = 0.0;
        if (p->color[1] <= 0.0) p->color[1] = 0.0;
        if (p->color[2] <= 0.0) p->color[2] = 0.0;

        // Check if the particle is entirely degraded and should be removed.
        if (p->color[0] <= 0.0 &&
            p->color[1] <= 0.0 &&
            p->color[2] <= 0.0
        ) {
            free(p);
            List_remove_current(particles);
        }
    }
}

/**
 * Explosions_draw
 * Draws the given particles.
 */
void Explosions_draw(List* particles)
{
    List_start_iteration(particles);
    Particle* p;
    while ((p = List_next(particles)))
    {

        glMatrixMode( GL_MODELVIEW );
        glPointSize(0.2);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, p->color);

        glBegin(GL_POINTS);
        glVertex3f(p->x, p->y, 0.0f);
        glEnd();

    }
}
