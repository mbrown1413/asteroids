
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "linked_list.h"
#include "alien.h"
#include "asteroid.h"
#include "bullet.h"
#include "explosion.h"
#include "player.h"
#include "game.h"
#include "interactions.h"

void print(float x, float y, char* text)
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

Game* Game_new(unsigned int starting_level)
{
    Game* g = (Game*) malloc(sizeof(Game));
    // Create asteroids, players, aliens
    g->aliens = List_new();
    g->asteroids = List_new();
    g->bullets = List_new();
    g->particles = List_new();
    g->player = Player_new(0,0, 0,1,0);

    //g->screen_width = 56;
    g->level = starting_level;
    g->level_start_timer = 0;

    Game_start_level(g, starting_level);
    return g;
}

void Game_draw(Game* g)
{
    print(0, 0, "HELLO");
}

void Game_update(Game* g)
{
    if (g->asteroids->length == 0 && g->aliens->length == 0)
    {
        g->level_start_timer++;
        if (g->level_start_timer > LEVEL_START_DELAY) {
            g->level += 1;
            g->level_start_timer = 0;
            Game_start_level(g, g->level);
        }
    }

    check_collisions(g->aliens, g->asteroids, g->bullets, g->particles,
        g->player, g->screen_width);

    //Alien_update(alien, g->screen_width);
    Asteroid_update_list(g->asteroids, g->screen_width);
    Bullet_update_list(g->bullets, g->screen_width);
    Explosions_update(g->particles, g->screen_width);
    Player_update(g->player, g->screen_width);

}

void Game_start_level(Game* g, unsigned int level)
{
    switch (level) {
        case 1:
            g->screen_width = 70;
            /*
            for (int i=0; i<2; i++) {
                List_append(g->asteroids, (void*) Asteroid_new_random(2, g->screen_width));
            }
            */
            for (int i=0; i<1; i++) {
                List_append(g->asteroids, (void*) Asteroid_new_random(1, g->screen_width));
            }
        break;
        case 2:
            g->screen_width += 40;
            /*
            for (int i=0; i<10; i++) {
                List_append(g->asteroids, (void*) Asteroid_new_random(1, g->screen_width));
            }
            */
            /*
            for (int i=0; i<2; i++) {
                List_append(g->asteroids, (void*) Asteroid_new_random(5, g->screen_width));
            }
            */
            for (int i=0; i<10; i++) {
                List_append(g->asteroids, (void*) Asteroid_new_random(1, g->screen_width));
            }
        break;
        case 3:
            g->screen_width += 100;
            for (int i=0; i<1; i++) {
                List_append(g->asteroids, (void*) Asteroid_new_random(10, g->screen_width));
            }
            /*
            for (int i=0; i<5; i++) {
                List_append(g->asteroids, (void*) Asteroid_new_random(5, g->screen_width));
            }
            */
        break;
    }
}

void Game_free(Game* g)
{
    //TODO
}
