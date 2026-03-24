// renderer is supposed to paint everything on screen (draw pixels)
#include "renderer.h"
#include "globals.h"
#include "objects.h"
#include "algorithms.h"
#include <GL/glut.h>
#include <string>

// ======================= LIGHTING ============================
void setupLighting()
{
    // activates one directional light source
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); //total 8 light sources are supported by opengl, i have enabled light 0

    glShadeModel(GL_SMOOTH); //interpolation (corners of cube may have different intensities so theyre smoothened)

    // (X, Y, Z, W) w=0 means directional (infinite, far) light, w=1 means point light
    GLfloat lpos[] = { 0.0f,  1.0f,  1.0f, 0.0f }; //(0, 1, 1) means light comes from above and forward
    GLfloat lamb[] = { 0.3f,  0.3f,  0.3f, 1.0f };
    GLfloat ldif[] = { 0.85f, 0.85f, 0.85f,1.0f };
    GLfloat lspc[] = { 0.2f,  0.2f,  0.2f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  lamb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  ldif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lspc);
}

void disableLighting()
{
    // disabling lighting for 2D
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
}

// ======================= TEXT ================================
void drawText(float x, float y, const std::string &s,
              float r, float g, float b)
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (char c : s)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

void drawTextLarge(float x, float y, const std::string &s,
                   float r, float g, float b)
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (char c : s)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
}

// PROJECTION matrix — controls the type of camera. Is it a flat 2D view or a 3D perspective view?
// MODELVIEW matrix — controls where objects are in the world, and where the camera is.
// ======================= HUD =================================
void drawHUD()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // navy background for HUD
    glColor3f(0.08f, 0.08f, 0.22f);
    glBegin(GL_QUADS);
      glVertex2f(-1.0f, 0.78f);
      glVertex2f( 1.0f, 0.78f);
      glVertex2f( 1.0f, 1.00f);
      glVertex2f(-1.0f, 1.00f);
    glEnd();

    drawText(-0.98f, 0.93f, "Score: " + std::to_string(score));
    drawText(-0.98f, 0.86f, "Lives: " + std::to_string(lives) + "/5");
    drawText(-0.28f, 0.93f, "Arrows:Move  P:Pause  R:Restart  T:3D  ESC:Exit");
    drawText( 0.72f, 0.93f, "Level: " + std::to_string(level) + "/" + std::to_string(MAX_LEVEL));
    drawText( 0.60f, 0.86f, "Diff:  " + difficulty);
    drawText( 0.60f, 0.79f, "3D: "    + std::string(enable3D ? "ON" : "OFF"));

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    if (enable3D)
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
    }
}

// ======================= PROJECTION ==========================
void setProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (enable3D)
    {
        // 56.74 — vertical field of view in degrees. Imagine opening your eyes wider or narrower. 56.74° is a natural-looking angle.
        // (double)WINDOW_W / WINDOW_H = 900/600 = 1.5 — the aspect ratio. Tells OpenGL the screen is 1.5 times wider than it is tall.
        // 0.01 — near clipping plane. Anything closer than 0.01 units to the camera is invisible.
        // 100.0 — far clipping plane. Anything beyond 100 units is invisible.
        gluPerspective(56.74, (double)WINDOW_W / WINDOW_H, 0.01, 100.0);
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        gluOrtho2D(-1, 1, -1, 1);
        glDisable(GL_DEPTH_TEST);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void force2D() //menu, game over (entirely 2d)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    disableLighting();
    glDisable(GL_DEPTH_TEST);
}

// ======================= DISPLAY =============================
void display()
{
    setProjection(); //First thing every frame — set up the correct camera (2D or 3D).
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Without this, every frame would draw on top of the previous frame
    glLoadIdentity();

    if (enable3D)
    {
        gluLookAt(0, 0, 1.85, 0, 0, 0, 0, 1, 0); //positions a virtual camera
        setupLighting(); //activates light source
    }
    else
    {
        disableLighting();
    }

    // ======== MENU ========
    if (gameState == MENU)
    {
        force2D();
        drawTextLarge(-0.32f,  0.60f, "BRICK BREAKER GAME",                                   0.3f, 0.9f, 1.0f);
        drawText     (-0.52f,  0.42f, "================================================");
        drawText     (-0.48f,  0.28f, "Select Difficulty (press before starting):");
        drawText     (-0.38f,  0.16f, "  1 - Easy       2 - Medium       3 - Hard");
        drawText     (-0.32f,  0.04f, "  T - Toggle 3D Bricks View");
        drawText     (-0.22f, -0.08f, "  S - Start Game");
        drawText     (-0.52f, -0.20f, "================================================");
        drawText     (-0.48f, -0.32f, "In Game:  Arrow Keys Move  |  P Pause  |  R Restart  |  ESC Exit");
        drawText     (-0.52f, -0.44f, "================================================");
        drawText     (-0.36f, -0.56f, "Difficulty : " + difficulty,                            1.0f, 0.9f, 0.3f);
        drawText     (-0.36f, -0.66f, "3D Mode    : " + std::string(enable3D ? "ON" : "OFF"),  0.3f, 1.0f, 0.5f);
    }

    // ======== COUNTDOWN ========
    else if (gameState == COUNTDOWN)
    {
        paddle.draw();
        ball.draw();
        brickGrid.draw();
        drawHUD();
        drawTextLarge(-0.04f,  0.05f, std::to_string(countdownNumber), 1.0f, 0.8f, 0.0f);
        drawText     (-0.22f, -0.10f, "Get Ready!",                    1.0f, 1.0f, 0.0f);
    }

    // ======== PLAYING ========
    else if (gameState == PLAYING)
    {
        paddle.draw();
        ball.draw();
        brickGrid.draw();
        drawHUD();
    }

    // ======== PAUSED ========
    else if (gameState == PAUSED)
    {
        paddle.draw();
        ball.draw();
        brickGrid.draw();
        drawHUD();
        drawTextLarge(-0.12f,  0.10f, "PAUSED",            1.0f, 1.0f, 0.0f);
        drawText     (-0.28f, -0.05f, "Press P to Resume", 1.0f, 1.0f, 1.0f);
    }

    // ======== LIFE LOST ========
    else if (gameState == LIFE_LOST)
    {
        paddle.draw();
        ball.draw();
        brickGrid.draw();
        drawHUD();
        drawTextLarge(-0.18f, 0.05f, "LIFE LOST!", 1.0f, 0.2f, 0.2f);
    }

    // ======== GAME OVER ========
    else if (gameState == GAME_OVER)
    {
        force2D();
        glColor3f(0.15f, 0.04f, 0.04f);
        drawRectangle(-0.65f, -0.70f, 0.65f, 0.75f);
        glColor3f(1.0f, 0.2f, 0.2f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
          glVertex2f(-0.65f, -0.70f);
          glVertex2f( 0.65f, -0.70f);
          glVertex2f( 0.65f,  0.75f);
          glVertex2f(-0.65f,  0.75f);
        glEnd();
        glLineWidth(1.0f);

        drawTextLarge(-0.22f,  0.55f, "GAME OVER!",                               1.0f, 0.2f, 0.2f);
        drawText     (-0.42f,  0.38f, "- - - - - - - - - - - - - - - - - - -",    0.8f, 0.3f, 0.3f);
        drawText     (-0.42f,  0.25f, "Final Score  : " + std::to_string(score),  1.0f, 1.0f, 0.4f);
        drawText     (-0.42f,  0.13f, "Level Reached: " + std::to_string(level),  1.0f, 1.0f, 0.4f);
        drawText     (-0.42f,  0.01f, "Difficulty   : " + difficulty,             1.0f, 1.0f, 0.4f);
        drawText     (-0.42f, -0.11f, "- - - - - - - - - - - - - - - - - - -",    0.8f, 0.3f, 0.3f);
        drawText     (-0.42f, -0.23f, "  R       - Return to Main Menu",           0.4f, 0.8f, 1.0f);
        drawText     (-0.42f, -0.35f, "  1/2/3   - Change Difficulty",             1.0f, 0.8f, 0.4f);
        drawText     (-0.42f, -0.47f, "  T       - Toggle 3D Mode",                0.8f, 0.6f, 1.0f);
        drawText     (-0.42f, -0.59f, "  ESC     - Exit Game",                     1.0f, 0.4f, 0.4f);
    }

    // ======== WIN ========
    else if (gameState == WIN_SCREEN)
    {
        force2D();

        glColor3f(0.04f, 0.15f, 0.06f);
        glBegin(GL_QUADS);
          glVertex2f(-0.65f, -0.70f);
          glVertex2f( 0.65f, -0.70f);
          glVertex2f( 0.65f,  0.75f);
          glVertex2f(-0.65f,  0.75f);
        glEnd();

        glColor3f(0.2f, 1.0f, 0.4f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
          glVertex2f(-0.65f, -0.70f);
          glVertex2f( 0.65f, -0.70f);
          glVertex2f( 0.65f,  0.75f);
          glVertex2f(-0.65f,  0.75f);
        glEnd();
        glLineWidth(1.0f);

        drawTextLarge(-0.18f,  0.55f, "YOU WIN!",                                  0.2f, 1.0f, 0.3f);
        drawTextLarge(-0.38f,  0.42f, "CONGRATULATIONS!",                          1.0f, 0.9f, 0.2f);
        drawText     (-0.42f,  0.25f, "Final Score   : " + std::to_string(score));
        drawText     (-0.42f,  0.15f, "Levels Cleared: " + std::to_string(level));
        drawText     (-0.42f,  0.05f, "Lives Left    : " + std::to_string(lives));
        drawText     (-0.42f, -0.10f, "Difficulty    : " + difficulty);
        drawText     (-0.42f, -0.30f, "Press S to Play Again");
        drawText     (-0.42f, -0.40f, "Press R for Main Menu");
        drawText     (-0.42f, -0.50f, "Press ESC to Exit");
    }

    glutSwapBuffers(); //brings the completed buffer back to the monitor so that everything appears to the user
    // and the old front buffer acts as a back buffer for next frame. cycle continues
}
