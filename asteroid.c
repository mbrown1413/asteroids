
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/gl.h>

#include "asteroid.h"
#include "crystal.h"
#include "explosion.h"
#include "linked_list.h"

#define PI 3.1415926535897932384626433832795028841971693993

float asteroid_vertices[18][3] = {
    {0.0f, 0.0f, 1.0f}, // Top

    // Upper Ring
    {0.0f, 1.0f, 0.33f}, // 1
    {-0.66f, 0.66f, 0.33f},
    {-1.0f, 0.0f, 0.33f},
    {-0.66f, -0.66f, 0.33f},
    {0.0f, -1.0f, 0.33f},
    {0.66f, -0.66f, 0.33f},
    {1.0f, 0.0f, 0.33f},
    {0.66f, 0.66f, 0.33f}, // 8

    {0.0f, 0.0f, -1.0f}, // Bottom

    // Lower Ring
    {0.0f, 1.0f, -0.33f}, // 10
    {-0.66f, 0.66f, -0.33f},
    {-1.0f, 0.0f, -0.33f},
    {-0.66f, -0.66f, -0.33f},
    {0.0f, -1.0f, -0.33f},
    {0.66f, -0.66f, -0.33f},
    {1.0f, 0.0f, -0.33f},
    {0.66f, 0.66f, -0.33f} // 17

};

/**
 * Asteroid_new
 * Creates a new asteroid object of the given parameters and returns it.
 *
 * Note that the random seed should be initialized before using this function:
 * srand( time(NULL) );
 */
Asteroid* Asteroid_new(
        float radius,
        float x, float y,
        float dx, float dy,
        float rot_dx, float rot_dy, float rot_dz
    ) {

    Asteroid* a = (Asteroid*) malloc(sizeof(Asteroid));
    if (!a) {
        printf("Could not allocate memory!\n");
        printf("    In Asteroid_new()\n");
        exit(1);
    }

    a->radius = radius;
    a->x = x;
    a->y = y;
    a->dx = dx;
    a->dy = dy;
    a->rot_x = 0;
    a->rot_y = 0;
    a->rot_z = 0;
    a->rot_dx = rot_dx;
    a->rot_dy = rot_dy;
    a->rot_dz = rot_dz;

    // Randomize asteroid
    float max_variance = 0.3f;
    float min_variance = 0.2f;
    float variance = min_variance + (max_variance-min_variance) * (rand()%100)/100.0f;
    for (int i=0; i<sizeof(asteroid_vertices)/sizeof(float); i++) {
        a->vertices[i/3][i%3] =
            asteroid_vertices[i/3][i%3] +
            variance * ((rand()%200)-100)/100.0f;
    }
    a->variance = variance;

    return a;
}

/**
 * Asteroid_new_random
 * Creates a new asteroid at random along the edge of the screen.
 */
Asteroid* Asteroid_new_random(float radius, float screen_width) {

    float x, y;
    if (rand()%2) {
        if (rand()%2) {
            // Top
            x = (rand()%1000 / 1000.0f) * screen_width - screen_width/2;
            y = screen_width/2 - radius;
        } else {
            // Bottom
            x = (rand()%1000 / 1000.0f) * screen_width - screen_width/2;
            y = -screen_width/2 + radius;
        }
    } else {
        if (rand()%2) {
            // Left
            x = -screen_width/2 + radius;
            y = (rand()%1000 / 1000.0f) * screen_width - screen_width/2;
        } else {
            // Right
            x = screen_width/2 - radius;
            y = (rand()%1000 / 1000.0f) * screen_width - screen_width/2;
        }
    }

    float dx = (rand()%2000 / 1000.0f - 1) / 5;
    float dy = (rand()%2000 / 1000.0f - 1) / 5;
    float rot_dx = rand()%200 / 100.0f - 1;
    float rot_dy = rand()%200 / 100.0f - 1;
    float rot_dz = rand()%200 / 100.0f - 1;

    // Sanity check of x, y
    if (x > screen_width/2) {
        x -= screen_width;
    } else if (x < -screen_width/2) {
        x += screen_width;
    }
    if (y > screen_width/2) {
        y -= screen_width;
    } else if (y < -screen_width/2) {
        y += screen_width;
    }

    return Asteroid_new(
        radius,
        x, y,
        dx, dy,
        rot_dx, rot_dy, rot_dz
    );
}

/**
 * Asteroid_split
 * Splits the asteroid into two.
 *
 * The asteroid passed into this function will be modified to be one of those
 * asteroids.  The other asteroid created will be returned.
 *
 * If the asteroid is vaporized instead of split, NULL is returned.  In this
 * case, the asteroid passed in should be removed.
 */
Asteroid* Asteroid_split(Asteroid* a, List* particles, List* crystals, float bullet_dx, float bullet_dy) {

    float explosion_color[] = {1.0f, 1.0f, 1.0f, 0.0f};
    Explosion_new(a->x, a->y, a->dx+0.2*bullet_dx, a->dy+0.2*bullet_dy, a->radius, explosion_color, particles);

    // Create Crystal
    //Crystal* crystal = Crystal_new(a->x, a->y, a->dx, a->dy);
    //List_append(crystals, crystal);

    if (a->radius < 1.5) {
        return NULL;
    }

    float delta_dx = (rand()%200 / 100.0 -1)/10;
    float delta_dy = (rand()%200 / 100.0 -1)/10;

    a->radius = a->radius*2/3;
    a->dx = a->dx;
    a->dy = a->dy;
    a->rot_dx = a->rot_dx;
    a->rot_dy = a->rot_dy;
    a->rot_dz = a->rot_dz;

    Asteroid* new_asteroid = Asteroid_new(
        a->radius,
        a->x, a->y,
        a->dx, a->dy,
        a->rot_dx, a->rot_dy, a->rot_dz
    );

    a->dx = a->dx + delta_dx;
    a->dy = a->dy + delta_dy;
    new_asteroid->dx = new_asteroid->dx - delta_dx;
    new_asteroid->dy = new_asteroid->dy - delta_dy;

    return new_asteroid;
}

/**
 * Asteroid_update
 * Updates the position and rotation of the asteroid.
 */
void Asteroid_update(Asteroid* a, float screen_width) {
    a->x += a->dx;
    a->y += a->dy;
    a->rot_x += a->rot_dx;
    a->rot_y += a->rot_dy;
    a->rot_z += a->rot_dz;

    if (fabs(a->x) > screen_width/2) {
        a->x = -a->x;
    }
    if (fabs(a->y) > screen_width/2) {
        a->y = -a->y;
    }
}

/**
 * Asteroid_update_list
 * Runs asteroid_update() on a list of asteroids with the given length.
 */
void Asteroid_update_list(List* asteroids, float screen_width) {
    List_start_iteration(asteroids);
    Asteroid* asteroid;
    while ((asteroid = (Asteroid*) List_next(asteroids)))
    {
        Asteroid_update(asteroid, screen_width);
    }
}

/**
 * Asteroid_draw
 * Draws the given asteroid.
 */
void Asteroid_draw(Asteroid* a) {
    //printf("x,y = (%f, %f)\n", a->x, a->y);

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();

    float asteroid_material[] = {0.2, 0.2, 0.2};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, asteroid_material);

    // Transforms
    glTranslatef(a->x, a->y, 0);
    glScalef(a->radius, a->radius, a->radius);
    glRotatef(a->rot_x, 1,0,0);
    glRotatef(a->rot_y, 0,1,0);
    glRotatef(a->rot_z, 0,0,1);

    // Asteroid vertex draw shortcut
    #define v(n) \
        glNormal3fv(a->vertices[n]); \
        glVertex3f(-a->vertices[n][0], -a->vertices[n][1], -a->vertices[n][2])

    // Top Fan
    glBegin(GL_TRIANGLE_FAN);
        for (int i=0; i<=8; i++) {
            v(i);
        }
        v(1);
    glEnd();

    glBegin(GL_TRIANGLES);
        for (int i=1; i<=7; i++) {
            v(i); v(i+9); v(i+1);
            v(i+1); v(i+9); v(i+10);
        }
        v(8); v(17); v(1);
        v(1); v(17); v(10);
        v(1); v(10); v(10);
        v(10); v(10); v(11);
    glEnd();

    // Bottom Fan
    glBegin(GL_TRIANGLE_FAN);
        for (int i=9; i<=17; i++) {
            v(i);
        }
        v(10);
    glEnd();

    #undef v

    glPopMatrix();

    glEnd();

}

/**
 * Asteroid_draw_list
 * Takes a list of asteroids and draws all of them.
 */
void Asteroid_draw_list(List* asteroids) {
    List_start_iteration(asteroids);
    Asteroid* asteroid;
    while ((asteroid = (Asteroid*) List_next(asteroids)))
    {
        Asteroid_draw(asteroid);
    }
}

void Asteroid_free(Asteroid* a) {
    free(a);
}
