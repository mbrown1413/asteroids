
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "crystal.h"
#include "linked_list.h"
#include "explosion.h"

Crystal* Crystal_new(float x, float y, float dx, float dy)
{
    Crystal* crystal = (Crystal*) malloc(sizeof(Crystal));
    if (!crystal) {
        printf("Could not allocate memory!\n");
        printf("    In Crystal_new()\n");
        exit(1);
    }
    crystal->x = x;
    crystal->y = y;
    crystal->dx = dx;
    crystal->dy = dy;

    crystal->spin_axis[0] = (rand()%2000 - 1000)/1000.0;
    crystal->spin_axis[1] = (rand()%2000 - 1000)/1000.0;
    crystal->spin_axis[2] = (rand()%2000 - 1000)/1000.0;
    crystal->spin_rate = (rand()%1000) / 1000.0;

    crystal->frames_to_live = 500;
    return crystal;
}

void Crystal_draw(Crystal* c)
{

    float crystal_material[] = {0.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, crystal_material);
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();

    // Transforms
    glTranslatef(c->x, c->y, 0);
    glRotatef(c->spin, c->spin_axis[0], c->spin_axis[1], c->spin_axis[2]);

    glutWireSphere(0.5, 5, 5);

    glPopMatrix();
}

void Crystal_draw_list(List* crystals)
{
    List_start_iteration(crystals);
    Crystal* crystal;
    while ((crystal = (Crystal*) List_next(crystals)))
    {
        Crystal_draw(crystal);
    }
}

bool Crystal_update(Crystal* c, float screen_width)
{
    c->x += c->dx;
    c->y += c->dy;
    if (fabs(c->x) > screen_width) c->x = -c->x;
    if (fabs(c->y) > screen_width) c->y = -c->y;

    c->spin += c->spin_rate;
    c->frames_to_live--;
    if (c->frames_to_live <= 0) {
        return false;
    }
    return true;
}

void Crystal_update_list(List* crystals, float screen_width)
{
    List_start_iteration(crystals);
    Crystal* crystal;
    while ((crystal = (Crystal*) List_next(crystals)))
    {
        bool result = Crystal_update(crystal, screen_width);
        if (!result)
        {
            List_remove_current(crystals);
        }
    }
}

void Crystal_explode(Crystal* c, List* particles)
{
}

void Crystal_free(Crystal* c)
{
    free(c);
}
