/* player.c
 * Handles the low level functions of a player.
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define PI 3.1415926535897932384626433832795028841971693993

#include "asteroid.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"

/**
 * Player_new
 * Creates a new player with the given position (x,y) and color (r,g,b).
 */
Player* Player_new(float x, float y, float r, float g, float b) {
    Player* p = (Player*) calloc(sizeof(Player), 1);
    if (!p) {
        printf("Could not allocate memory!\n");
        printf("    In Player_new()\n");
        exit(1);
    }

    p->mat[0] = r;
    p->mat[1] = g;
    p->mat[2] = b;
    p->mat[3] = 0.0;
    p->dead = true;

    p->weapon_cool_point = 80;
    p->weapon_heat = 0;
    p->weapon_fire_heat = 15;

    p->extra_lives = 3;
    p->score = 0;
    p->spawn_timer = 50;

    return p;
}

/**
 * Player_draw
 * Draws the given player.
 */
void Player_draw(Player* p) {
    if (p->dead) return;

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, p->mat);
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();

    // Transforms
    glTranslatef(p->x, p->y, 0);
    glRotatef(p->yaw, 0,0,1);
    glRotatef(90, 0, 1, 0);

    glutSolidCone(0.5, 2.0, 10, 2);

    // Draw aimer
    /*
    glBegin(GL_LINE_STRIP);
    float color[4];
    color[0] = p->mat[0]+0.3;
    color[1] = p->mat[1]-0.5;
    color[2] = p->mat[2]+0.3;
    color[3] = p->mat[3];
    for (int i=0; i<10; i++) {
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
        color[0] -= 0.3/10;
        color[1] -= 0.5/10;
        color[2] -= 0.3/10;
        glVertex3f(0, 0, i*1.5);
    }
    glEnd();
    */

    glPopMatrix();
}

/**
 * Player_fire
 * Fires a bullet from the player's ship.
 *
 * Returns a bullet if the fire was successful.  Returns NULL if the player
 * cannot fire a bullet at this time for some reason.
 */
Bullet* Player_fire(Player* p) {
    if (p->dead) return NULL;
    if (p->weapon_heat >= p->weapon_cool_point) {
        return NULL;
    }
    p->weapon_heat += p->weapon_fire_heat;
    Bullet* b = Bullet_new(
        p->x,
        p->y,
        BULLET_VELOCITY * cos((PI/180)*p->yaw) + p->dx,
        BULLET_VELOCITY * sin((PI/180)*p->yaw) + p->dy,
        BULLET_LIFETIME,
        p->mat[0], p->mat[1], p->mat[2],
        (void*) p
    );
    return b;
}

/**
 * Player_update
 * Updates the player.  Wraps at the given screen_width.  To be called every
 * frame.
 */
void Player_update(Player* p, float screen_width) {

    // Cooldown Weapon
    if (p->weapon_heat > 0) {
        p->weapon_heat--;
    }

    if (p->dead) {
        p->spawn_timer--;
        if (p->extra_lives > 0) {

            if (p->spawn_timer <= 0) {
                Player_spawn(p, 0, 0);
            }
        }

    } else { // not dead

        // Update Position
        p->x += p->dx;
        p->y += p->dy;
        p->yaw += p->dyaw * TURN_RATE;

        // Sanity check of x, y
        if (p->x > screen_width/2) {
            p->x -= screen_width;
        } else if (p->x < -screen_width/2) {
            p->x += screen_width;
        }
        if (p->y > screen_width/2) {
            p->y -= screen_width;
        } else if (p->y < -screen_width/2) {
            p->y += screen_width;
        }

        // Update velocity dx, dy
        if (p->thrust) {
            p->dx += cos((PI/180)*p->yaw) * THRUST_ACCELERATION;
            p->dy += sin((PI/180)*p->yaw) * THRUST_ACCELERATION;

            // Cap velocity
            #ifdef CAP_VELOCITY
                float magnitude = sqrt(p->dx*p->dx + p->dy*p->dy);
                if (magnitude > MAXIMUM_VELOCITY) {
                    // Shorten the vector to it's maximum length
                    // Divide by magnitude to get unit vector, then multiply by
                    // MAXIMUM_VELOCITY
                    p->dx = p->dx / magnitude * MAXIMUM_VELOCITY;
                    p->dy = p->dy / magnitude * MAXIMUM_VELOCITY;
                }
            #endif
        }

        // Friction
        #ifdef CAP_VELOCITY
            p->dx *= 0.99;
            p->dy *= 0.99;
        #endif

    }

}

/**
 * Player_destroy_asteroid
 * Called when the player destroys an asteroid either by a bullet or by running
 * into the asteroid.
 *
 * TODO: Don't think this is used.
 */
void Player_destroy_asteroid(Player* p, Asteroid* asteroid)
{
}

/**
 * Player_spawn
 * TODO
 */
void Player_spawn(Player* p, float x, float y)
{
    p->x = x;
    p->y = y;
    p->dx = 0;
    p->dy = 0;
    p->yaw = 90;
    p->dyaw = 0;

    p->weapon_heat = 0;

    p->dead = false;
    p->extra_lives--;
    p->spawn_timer = 0;
}

/**
 * Player_die
 * Kills the given player.
 */
void Player_die(Player* p, List* particles)
{
    if (p->dead) return;
    Explosion_new(p->x, p->y, p->dx,
        p->dy, 1, p->mat, particles);
    p->dead = true;
    p->spawn_timer = 100;
}
