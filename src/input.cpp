#include "input.h"
#include "globals.h"
#include "objects.h"
#include <GL/glut.h>
#include <cstdlib>

// ======================= KEYBOARD ============================
void keyboard(unsigned char key, int, int)
{
    switch (key)
    {

    case 's':
    case 'S':
        if (gameState == MENU)
        {
            score = 0;
            lives = 5;
            livesLost = 0;
            level = 1;
            ball.dx = 0.012f;
            ball.dy = -0.016f;
            ball.resetPosition();
            brickGrid.reset();
            paddle.x = 0.0f;
            countdownStartTime = glutGet(GLUT_ELAPSED_TIME);
            gameState = COUNTDOWN;
        }
        else if (gameState == WIN_SCREEN)
        {
            score = 0;
            lives = 5;
            livesLost = 0;
            level = 1;
            difficulty = "Easy";
            ball.dx = 0.012f;
            ball.dy = -0.016f;
            ball.resetPosition();
            brickGrid.reset();
            paddle.x = 0.0f;
            countdownStartTime = glutGet(GLUT_ELAPSED_TIME);
            gameState = COUNTDOWN;
        }
        break;

    case 'p':
    case 'P':
        if (gameState == PLAYING)
            gameState = PAUSED;
        else if (gameState == PAUSED)
            gameState = PLAYING;
        break;

    case 'r':
    case 'R':
        score = 0;
        lives = 5;
        livesLost = 0;
        level = 1;
        difficulty = "Easy";
        ball.dx = 0.012f;
        ball.dy = -0.016f;
        ball.resetPosition();
        brickGrid.reset();
        paddle.x     = 0.0f;
        rotationAngle = 0.0f;
        keyLeft  = false;
        keyRight = false;
        gameState = MENU;
        break;

    case 't':
    case 'T':
        enable3D = !enable3D;
        break;

    case '1':
        difficulty = "Easy";
        ball.dx = 0.012f;
        ball.dy = (ball.dy < 0 ? -0.016f : 0.016f);
        break;
    case '2':
        difficulty = "Medium";
        ball.dx = 0.022f;
        ball.dy = (ball.dy < 0 ? -0.026f : 0.026f);
        break;
    case '3':
        difficulty = "Hard";
        ball.dx = 0.032f;
        ball.dy = (ball.dy < 0 ? -0.036f : 0.036f);
        break;

    case 27:
        exit(0);
        break;
    }
}

// ======================= SPECIAL KEYS ========================
// arrow keys don't have ascii codes so glut_key_left, right are used
void specialKeys(int key, int, int)
{
    if (key == GLUT_KEY_LEFT)  keyLeft  = true;
    if (key == GLUT_KEY_RIGHT) keyRight = true;
}

void specialKeysUp(int key, int, int)
{
    if (key == GLUT_KEY_LEFT)  keyLeft  = false;
    if (key == GLUT_KEY_RIGHT) keyRight = false;
}