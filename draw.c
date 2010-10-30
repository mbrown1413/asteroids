
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "alien.h"
#include "asteroid.h"
#include "bullet.h"
#include "draw.h"
#include "explosion.h"
#include "player.h"
#include "game.h"

#define PI 3.14159265358979

#define FOV_DEGREES 70

// Static variables
float camera_distance = -1;
int zoom_square_width;
int window_height;
int window_width;
int view_width;

/**
 * draw_text
 * A helper function that draws the text on the screen at the given position.
 */
void draw_text(float x, float y, char* text)
{
    float text_color[] = {1.0, 0.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, text_color);
    glColor4f(1, 1, 1, 1);
    glRasterPos2f(x,y);
    while (*text != '\0') {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *text);
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
        view_width = window_height;
    } else {
        view_width = window_width;
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
void draw_zoom_square(Game* game, float desired_camera_distance)
{
    if (fabs(desired_camera_distance - camera_distance) > 0.5) {
        // Zoom square is visible

        // Move camera closer to desired position
        camera_distance += 0.03*(desired_camera_distance - camera_distance);

        // Draw square
        float alpha = ((desired_camera_distance - camera_distance))/100.0;
        float square_mat[] = {alpha, alpha, alpha}; // Color will be grey
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, square_mat);
        glBegin(GL_LINE_LOOP);
        glVertex3f(-zoom_square_width/2, -zoom_square_width/2, -1.0);
        glVertex3f(zoom_square_width/2, -zoom_square_width/2, -1.0);
        glVertex3f(zoom_square_width/2, zoom_square_width/2, -1.0);
        glVertex3f(-zoom_square_width/2, zoom_square_width/2, -1.0);
        glEnd();

    } else { // Zoom square NOT visible
        camera_distance = desired_camera_distance;
        zoom_square_width = game->screen_width;
    }
}

/**
 * draw_all
 * The master draw function that draws everything.
 */
void draw_all(Game* game) {

    // Determine Camera distance
    float desired_camera_distance = 2*(game->screen_width) / (tan((PI/180.0)*FOV_DEGREES));
    if (camera_distance == -1) {
        camera_distance = desired_camera_distance;
    }

    // Setup projection/camera
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0.0, 0.0, view_width, view_width);
    gluPerspective(FOV_DEGREES, // Field of view
                   1.0,         // Aspect ratio
                   1, 4000.0);    // Near and far
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0.0, 0, camera_distance,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    draw_objects(game);
    draw_zoom_square(game, desired_camera_distance);

    // Setup projection for HUD
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10, 10, -10, 10, 1, 10);

    draw_hud(game);

    glFlush();
    glutSwapBuffers();

}

/**
 * draw_objects
 * Draws all physical objects, but not HUD text.
 */
void draw_objects(Game* game) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Draw everything 9 times
    //
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
            //Alien_draw(alien);
            Asteroid_draw_list(game->asteroids);
            Bullet_draw_list(game->bullets, game->screen_width);
            Explosions_draw(game->particles);
            Player_draw(game->player);

            glPopMatrix();

        }
    }

}

/**
 * draw_hud
 * Draws the HUD (Heads Up Display).
 */
void draw_hud(Game* game) {
    draw_text(0, 0, "HELLO");
}
