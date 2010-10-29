
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "alien.h"

/**
 * Alien_new
 * Creates a new alien.
 */
Alien* Alien_new() {
    Alien* a = calloc(sizeof(Alien), 1);
    a->x = -7;
    a->y = 10;
    a->yaw = 0;
    a->dyaw = 3;
    return a;
}

/**
 * Alien_update
 * Updates the given alien.
 */
void Alien_update(Alien* a, float screen_width) {
    a->yaw += a->dyaw;
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
    //glRotatef(rotate, 4,2,1);
    glRotatef(-45, 1, 0, 0);
    glRotatef(a->yaw, 0, 0, 1);
    glScalef(0.5, 0.5, 0.5);

    glutSolidTorus(2, 5, 30, 5);
    glutSolidTorus(0.5, 2.5, 30, 5);
    glutSolidTorus(1, .1, 30, 5);

    glPopMatrix();
}

/**
 * Alien_free
 * Frees the memory for the given Alien.
 */
void Alien_free(Alien* alien) {
    free(alien);
}
