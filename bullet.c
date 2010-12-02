
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "bullet.h"
#include "linked_list.h"

/**
 * Bullet_new
 * Creates a new bullet with the given parameters.
 */
Bullet* Bullet_new(
    float x, float y,
    float dx, float dy,
    int frames_to_live,
    float r, float g, float b,
    void* owner
) {
    Bullet* bullet = (Bullet*) malloc(sizeof(Bullet));
    if (!bullet) {
        printf("Could not allocate memory!\n");
        printf("    In Bullet_new()\n");
        exit(1);
    }
    bullet->x = x;
    bullet->y = y;
    bullet->dx = dx;
    bullet->dy = dy;
    bullet->color[0] = r;
    bullet->color[1] = g;
    bullet->color[2] = b;
    bullet->frames_to_live = frames_to_live;
    bullet->owner = owner;
    return bullet;
}

/**
 * Bullet_draw
 * Draws the given bullet.
 */
void Bullet_draw(Bullet* b, float screen_width)
{
    glMatrixMode( GL_MODELVIEW );
    glPointSize(300.0/screen_width);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, b->color);

    glBegin(GL_POINTS);
    glVertex3f(b->x, b->y, 0.0f);
    glEnd();
}

/**
 * Bullet_draw_list
 * Draws the given list of bullets.
 */
void Bullet_draw_list(List* bullets, float screen_width)
{
    List_start_iteration(bullets);
    Bullet* bullet;
    while ((bullet = (Bullet*) List_next(bullets)))
    {
        Bullet_draw(bullet, screen_width);
    }
}

/**
 * Bullet_update_list
 * Takes a List* of bullets and updates them all.  Removes bullet if they are done firing.
 */
void Bullet_update_list(List* bullets, float screen_width)
{
    List_start_iteration(bullets);
    Bullet* bullet;
    while ((bullet = (Bullet*) List_next(bullets)))
    {
        bool result = Bullet_update(bullet, screen_width);
        if (result == false) {
            Bullet_free(bullet);
            List_remove_current(bullets);
        }
    }
}

/**
 * Bullet_update
 * Updates a single bullet.  Returns false if the bullet is past it's frames_to_live.
 */
bool Bullet_update(Bullet* b, float screen_width)
{
        b->frames_to_live--;
        if (b->frames_to_live < 0) {
            return false;
        }
        b->x += b->dx;
        b->y += b->dy;
        if (fabs(b->x) > screen_width) b->x = -b->x;
        if (fabs(b->y) > screen_width) b->y = -b->y;
        return true;
}

/**
 * Bullet_free
 * Frees the memory for the given Bullet.
 */
void Bullet_free(Bullet* bullet) {
    free(bullet);
}
