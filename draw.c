
#include <math.h>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "alien.h"
#include "asteroid.h"
#include "bullet.h"
#include "crystal.h"
#include "draw.h"
#include "explosion.h"
#include "player.h"
#include "game.h"

#define PI 3.14159265358979

// Static variables
int zoom_square_width;
int window_height;
int window_width;
float view_width = 0;

/**
 * draw_text
 * A helper function that draws the text on the screen at the given position.
 */
void draw_text(float x, float y, void* font, char* text)
{
    float text_color[] = {0.0, 1.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, text_color);
    glColor4f(1, 1, 1, 1);
    glRasterPos2f(x,y);
    while (*text != '\0') {
        glutBitmapCharacter(font, *text);
        text++;
    }
}

/**
 * reshape
 * Callback for when the window is resized.
 */
void reshape(int w, int h)
{
    window_width = w;
    window_height = h;

    // Clip the viewport so it's a square within the window.
    if (window_width > window_height) {
        window_width = window_height;
    } else {
        window_width = window_width;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**
 * draw_zoom_square
 * Draws a fading square while zooming out.
 *
 * This is meant to put things in perspective while no asteroids are around.
 */
void draw_zoom_square(Game* game, float desired_view_width)
{
    if (fabs(desired_view_width - view_width) > 0.5) {
        // Zoom square is visible

        // Move camera closer to desired position
        view_width += 0.06*(desired_view_width - view_width);

        // Draw square
        float alpha = ((desired_view_width - view_width))/100.0;
        float square_mat[] = {alpha, alpha, alpha}; // Color will be grey
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, square_mat);
        glBegin(GL_LINE_LOOP);
        glVertex3f(-zoom_square_width/2, -zoom_square_width/2, -1.0);
        glVertex3f(zoom_square_width/2, -zoom_square_width/2, -1.0);
        glVertex3f(zoom_square_width/2, zoom_square_width/2, -1.0);
        glVertex3f(-zoom_square_width/2, zoom_square_width/2, -1.0);
        glEnd();

    } else { // Zoom square NOT visible
        view_width = desired_view_width;
        zoom_square_width = game->screen_width;
    }
}

/**
 * draw_all
 * The master draw function that draws everything.
 */
void draw_all(Game* game) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw_hud(game);
    draw_objects(game);
    draw_zoom_square(game, game->screen_width);

    glFlush();
    glutSwapBuffers();

}

/**
 * draw_objects
 * Draws all physical objects, but not HUD text.
 */
void draw_objects(Game* game) {

    // Setup projection/camera
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0.0, 0.0, window_width, window_width);
    glOrtho(-view_width/2, view_width/2, -view_width/2, view_width/2, 0.0, 4000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0.0, 0, 10,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    // Draw everything 9 times
    // This creates the screen wrapping effect.  When something moves off of
    // the screen in one direction, another drawing enters on the opposite
    // side.  When updating x and y for each item, they have to add or subtract
    // their screen width at some point, but it isn't sensitive exactly when.
    for (int x_offset=-1; x_offset<=1; x_offset++) {
        for (int y_offset=-1; y_offset<=1; y_offset++) {

            glMatrixMode( GL_MODELVIEW );
            glPushMatrix();
            glTranslatef(
                game->screen_width*x_offset,
                game->screen_width*y_offset,
                0
            );

            // Actual Drawing
            Asteroid_draw_list(game->asteroids);
            Bullet_draw_list(game->bullets, game->screen_width);
            Crystal_draw_list(game->crystals);
            Explosions_draw(game->particles);
            Player_draw(game->player);

            glPopMatrix();

        }
    }

    // Non wrapping objects
    Alien_draw_list(game->aliens);

}

/**
 * draw_hud
 * Draws the HUD (Heads Up Display).
 */
void draw_hud(Game* game) {
    Player* player = game->player;

    // Setup projection/camera
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0.0, 0.0, window_width, window_width);
    glOrtho(-10, 10, -10, 10, 1, 4000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0.0, 0, 10,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    char text[50];

    // Lives
    sprintf(text, "EXTRA LIVES: %d", game->player->extra_lives);
    draw_text(-9.5, 9.0, GLUT_BITMAP_9_BY_15, text);

    // Points
    sprintf(text, "SCORE: %d", game->player->score);
    draw_text(-4.5, 9.0, GLUT_BITMAP_9_BY_15, text);

    // Game Over
    if (game->player->dead && game->player->spawn_timer <= 0)
    {
        if (abs(game->player->spawn_timer)%60 > 20) { // Blink
            draw_text(-2.2, 0, GLUT_BITMAP_TIMES_ROMAN_24, "GAME OVER");
        }
    }

    // Weapon Cooldown Meter
    glScalef(0.09, 1, 1);
    glTranslatef(2.5, 9.2, 0);
    float hot_color[] = {1.0, 0.0, 0.0, 1.0};
    float cold_color[] = {0.0, 1.0, 0.0, 1.0};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, hot_color);
    glBegin(GL_LINE_STRIP);

        glVertex2f(0, 0);
        glVertex2f(player->weapon_heat, 0);

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, cold_color);
        glVertex2f(player->weapon_cool_point + player->weapon_fire_heat, 0);

    glEnd();
}
