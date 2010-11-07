
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "linked_list.h"
#include "alien.h"
#include "asteroid.h"
#include "bullet.h"
#include "draw.h"
#include "explosion.h"
#include "player.h"
#include "game.h"
#include "collisions.h"

#define MENU_ITEM_NEW_GAME 0
#define MENU_ITEM_QUIT 1

void update(int d);

Game* game;

void update(int d) {
    glutTimerFunc(20, update, 0);
    Game_update(game);
    glutPostRedisplay();

    //camera_distance += 0.1;
    //screen_width = 0.5 * camera_distance*tan((PI/180.0) * FOV_DEGREES) + 1;
}

void draw() {
    draw_all(game); // draw.c
}

void handle_menu(int item) {
    switch (item) {
        case MENU_ITEM_QUIT:
            Game_free(game);
            exit(0);
        break;
        case MENU_ITEM_NEW_GAME:
            Game_free(game);
            game = Game_new(1);
        break;
    }
}

void key_special_up(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            game->player->dyaw = 0;
        break;
        case GLUT_KEY_RIGHT:
            game->player->dyaw = 0;
        break;
        case GLUT_KEY_UP:
            game->player->thrust = 0;
        break;
    }
}

void key_special_down(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            game->player->dyaw = 1;
        break;
        case GLUT_KEY_RIGHT:
            game->player->dyaw = -1;
        break;
        case GLUT_KEY_UP:
            game->player->thrust = 1;
        break;
    }
}

void key_down(unsigned char key, int x, int y) {
    Bullet* b;
    switch (key) {
        case 'x':
            game->player->thrust = 1;
        break;
        case 'z':
        case ' ':
            b = Player_fire(game->player);
            if (b != NULL) {
                List_append(game->bullets, (void*) b);
            }
        break;
        case 'h':
            game->player->dyaw = 1;
        break;
        case 'l':
            game->player->dyaw = -1;
        break;
    }
}

void key_up(unsigned char key, int x, int y) {
    switch (key) {
        case 'x':
            game->player->thrust = 0;
        break;
        case 'h':
            game->player->dyaw = 0;
        break;
        case 'l':
            game->player->dyaw = 0;
        break;
    }
}

int main(int argc, char** argv)
{
    srand( time(NULL) );

    // Window Initialization
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(argv[0]);

    // OpenGL settings
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    // Initialize lighting
    float ambience[] = {0, 0, 0, 0};
    float light0_position[] = {0, 1, -0.1, 0};
    float light0_ambience[] = {1, 1, 1, 1};
    float light0_diffuse[] = {.8, .8, .8, 1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambience);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambience);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_POINT_SMOOTH);

    // Register Functions
    glutDisplayFunc(draw);
    glutReshapeFunc(reshape); // draw.c
    glutTimerFunc(0, update, 0);

    // Key Functions
    glutSpecialFunc( key_special_down );
    glutSpecialUpFunc( key_special_up );
    glutKeyboardFunc( key_down );
    glutKeyboardUpFunc( key_up );

    // Menu
    glutCreateMenu(handle_menu);
    glutAddMenuEntry("New Game", MENU_ITEM_NEW_GAME);
    glutAddMenuEntry("Quit", MENU_ITEM_QUIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    game = Game_new(1);

    glutMainLoop();

    return 0;
}
