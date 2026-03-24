// cd src
// g++ main.cpp algorithms.cpp objects.cpp renderer.cpp input.cpp -o app -lfreeglut -lopengl32 -lglu32
// .\app.exe
// =============================================================
// BRICK BREAKER GAME - Enhanced with 3D, Transformations & More
// =============================================================

// Game execution per frame:

// 1. timer() runs
// 2. Ball::move() updates position
// 3. BrickGrid::checkCollision()
// 4. Paddle collision checked
// 5. display() called
// 6. draw paddle
// 7. draw ball
// 8. draw bricks
// 9. swap buffers

// This repeats ~60 times per second.
// So the player sees continuous motion.

#include "globals.h"
#include "objects.h"
#include "renderer.h"
#include "input.h"
#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <cmath>

// ======================= GLOBAL VARIABLE DEFINITIONS =========
// (declared extern in globals.h, defined exactly once here)
GameState gameState = MENU;
int score = 0;
int lives = 5;
int livesLost = 0;
int level = 1;
std::string difficulty = "Easy";

int lifeLostMessageTime = 0;
int countdownStartTime = 0;
int countdownNumber = 3;

bool enable3D = false;
float rotationAngle = 0.0f;

bool keyLeft = false;
bool keyRight = false;

// ======================= GAME OBJECTS ========================
Paddle paddle;
Ball ball;
BrickGrid brickGrid;

// ======================= COLLISIONS ==========================
// The paddle occupies from y = -0.92 to y = -0.87 in normalized coordinates (the window goes from -1 to +1 on both axes).
void checkCollisions()
{
    if (gameState != PLAYING)
        return;

    // Ball vs paddle
    // ball.x, ball.y is centre of the ball
    // left edge < ball.x < right edge
    if (ball.y - ball.radius <= -0.87f &&
        ball.y - ball.radius >= -0.95f &&
        ball.x > paddle.x - paddle.width / 2 &&
        ball.x < paddle.x + paddle.width / 2)
    {
        ball.dy = fabsf(ball.dy);
        // ball moves UP after hitting paddle
        score += 5;
    }

    // Ball vs bricks
    brickGrid.checkCollision(ball);
}

// ======================= TIMER ===============================
// elapsed represents time passed since last frame.
void timer(int)
{
    if (gameState == COUNTDOWN)
    {
        // (End Time - Start Time = Elapsed Time)
        // elapsed ms since program start
        int elapsed = glutGet(GLUT_ELAPSED_TIME) - countdownStartTime;
        if (elapsed < 1000)
            countdownNumber = 3;
        else if (elapsed < 2000)
            countdownNumber = 2;
        else if (elapsed < 3000)
            countdownNumber = 1;
        else
            gameState = PLAYING;
    }
    else if (gameState == LIFE_LOST)
    {
        if (keyLeft)
            paddle.moveLeft();
        if (keyRight)
            paddle.moveRight();
        rotationAngle += 0.8f;
        if (rotationAngle > 360.0f)
            rotationAngle -= 360.0f;
        int elapsed = glutGet(GLUT_ELAPSED_TIME) - lifeLostMessageTime;
        if (elapsed >= 1000)
            gameState = PLAYING;
    }
    else if (gameState == PLAYING)
    {
        if (keyLeft)
            paddle.moveLeft();
        if (keyRight)
            paddle.moveRight();
        ball.move();
        checkCollisions();
        rotationAngle += 0.8f;
        if (rotationAngle > 360.0f)
            rotationAngle -= 360.0f;

        // WIN / NEXT LEVEL CHECK
        if (gameState == PLAYING && brickGrid.allCleared())
        {
            if (level >= MAX_LEVEL)
            {
                ball.dx = 0;
                ball.dy = 0;
                brickGrid.reset();
                gameState = WIN_SCREEN;
            }
            else
            {
                level++;
                brickGrid.reset();
                ball.resetPosition();
                countdownStartTime = glutGet(GLUT_ELAPSED_TIME);
                gameState = COUNTDOWN;
            }
        }
    }

    glutPostRedisplay();// called every 16ms by timer(). it calls display()
    glutTimerFunc(16, timer, 0);  //timer() updates game state every 16ms
}

// ======================= INIT ================================
void init()
{
    glPointSize(2.0f);
    glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
    srand((unsigned)time(0));
}

// ======================= MAIN ================================
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //configures framebuffer
    // glut_double is double buffering (two canvases analogy), glut rgb : standard color, glut_depth depth buffer (for 3d mode)
    glutInitWindowSize(WINDOW_W, WINDOW_H);
    glutCreateWindow("Brick Breaker - CG Project");

    init(); //sets bg color and pixel size
    glutDisplayFunc(display); //"when the window needs repainting, call display()
    glutKeyboardFunc(keyboard); //when a regular ASCII key is pressed, call keyboard()

    // arrow keys and non ascii keys
    glutSpecialFunc(specialKeys);
    glutSpecialUpFunc(specialKeysUp);

    // timer is called every 16ms
    glutTimerFunc(16, timer, 0);
    glutMainLoop(); //hands control to GLUT permanently. due to this, code will run only when GLUT calls sth
    return 0;
}
