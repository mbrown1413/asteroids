
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

Game* Game_new(unsigned int starting_level)
{
    Game* g = (Game*) malloc(sizeof(Game));
    if (!g) {
        printf("Could not allocate memory!\n");
        printf("    In Game_new()\n");
        exit(1);
    }
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

    check_collisions(g);

    Alien_update_list(g->aliens, g);
    Asteroid_update_list(g->asteroids, g->screen_width);
    Bullet_update_list(g->bullets, g->screen_width);
    Explosions_update(g->particles, g->screen_width);
    Player_update(g->player, g->screen_width);

}

void Game_start_level(Game* g, unsigned int level)
{
    Alien* alien;
    switch (level) {
        case 1:
            g->screen_width = 70;
            /*
            alien = Alien_new(false, g->screen_width);
            List_append(g->aliens, (void*) alien);
            */
            /*
            for (int i=0; i<2; i++) {
                List_append(g->asteroids, (void*) Asteroid_new_random(2, g->screen_width));
            }
            for (int i=0; i<1; i++) {
                List_append(g->asteroids, (void*) Asteroid_new_random(5, g->screen_width));
            }
            */
            for (int i=0; i<3; i++) {
                List_append(g->asteroids, (void*) Asteroid_new_random(1, g->screen_width));
            }
        break;
        case 2:
            g->screen_width += 40;
            for (int i=0; i<2; i++) {
                List_append(g->asteroids, (void*) Asteroid_new_random(1, g->screen_width));
            }
            for (int i=0; i<3; i++) {
                List_append(g->asteroids, (void*) Asteroid_new_random(5, g->screen_width));
            }
        break;
        case 3:
            g->screen_width += 70;
            for (int i=0; i<4; i++) {
                List_append(g->asteroids, (void*) Asteroid_new_random(5, g->screen_width));
            }
            for (int i=0; i<2; i++) {
                List_append(g->asteroids, (void*) Asteroid_new_random(1, g->screen_width));
            }
        break;
        default:
            for (int i=0; i<6; i++) {
                List_append(g->asteroids, (void*) Asteroid_new_random(5, g->screen_width));
            }
        break;
    }
}

void Game_free(Game* g)
{

    // Aliens
    List_start_iteration(g->aliens);
    Alien* alien;
    while ((alien = (Alien*) List_next(g->aliens))) {
        Alien_free(alien);
    }

    // Asteroids
    List_start_iteration(g->asteroids);
    Asteroid* asteroid;
    while ((asteroid = (Asteroid*) List_next(g->asteroids))) {
        Asteroid_free(asteroid);
    }

    // Bullets
    List_start_iteration(g->bullets);
    Bullet* bullet;
    while ((bullet = (Bullet*) List_next(g->bullets))) {
        Bullet_free(bullet);
    }

    // Particles
    List_start_iteration(g->particles);
    Particle* particle;
    while ((particle = (Particle*) List_next(g->particles))) {
        free(particle);
    }

}
