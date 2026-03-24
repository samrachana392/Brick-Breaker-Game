#include "objects.h"
#include "algorithms.h"
#include "globals.h"
#include <GL/glut.h>
#include <cmath>

// ======================= PADDLE ==============================
void Paddle::moveLeft()
{
    if (x - width / 2 > -1.0f)
        x -= 0.025f;
    // x is paddle center position.
    // width/2 is half paddle width.
    // ensures paddle doesnt move past left boundary
}

void Paddle::moveRight()
{
    if (x + width / 2 < 1.0f)
        x += 0.025f;
}

void Paddle::draw()
{
    glColor3f(0.22f, 1.0f, 0.08f); // neon lime
    if (enable3D)
    {
        glPushMatrix();
        glTranslatef(x, -0.895f, 0.0f);
        glScalef(width, 0.05f, 0.08f);
        glutSolidCube(1.0f);
        glPopMatrix();
    }
    else
    {
        float x1 = x - width / 2;
        float x2 = x + width / 2;
        float y1 = -0.92f;
        float y2 = -0.87f;

        glColor3f(0.22f, 1.0f, 0.08f); // neon lime
        drawRectangle(x1, y1, x2, y2);
    }
}

// ======================= BALL ================================
void Ball::move()
{
    if (gameState != PLAYING)
        return;

    x += dx;
    // new position = old position + velocity
    y += dy;

    if (x + radius > 1.0f)
    {
        x = 1.0f - radius;
        dx *= -1;
    }
    // x + radius   checks right edge of ball.
    // If it crosses boundary: velocity reverses

    if (x - radius < -1.0f)
    {
        x = -1.0f + radius;
        dx *= -1;
    }
    if (y + radius > 0.78f)
    {
        y = 0.78f - radius;
        dy *= -1;
    }

    if (y - radius < -1.0f)
    {
        // ball fell below screen
        lives--;
        livesLost++;
        if (lives <= 0)
        {
            gameState = GAME_OVER;
        }
        else
        {
            y = -1.0f + radius;
            dy = fabsf(dy);
            gameState = LIFE_LOST;
            lifeLostMessageTime = glutGet(GLUT_ELAPSED_TIME);
        }
    }
}

void Ball::resetPosition()
{
    x = 0.0f;
    y = 0.50f;
    if (dy > 0)
        dy *= -1;
}

void Ball::draw()
{
    glColor3f(1.0f, 1.0f, 0.0f);
    if (enable3D)
    {
        glPushMatrix();
        glTranslatef(x, y, 0.0f);

         GLfloat amb[]  = {1.0f, 0.81f, 0.0f, 1.0f}; // dark yellow ambient
        GLfloat diff[] = {1.0f, 0.90f, 0.0f, 1.0f}; // hot yellow diffuse
        GLfloat spec[] = {1.0f, 1.0f,  0.8f, 1.0f}; // white-ish shine
        GLfloat shin   = 60.0f;                       // tight shiny spot

        glMaterialfv(GL_FRONT, GL_AMBIENT,   amb);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   diff);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  spec);
        glMaterialf (GL_FRONT, GL_SHININESS, shin);

        glutSolidSphere(radius * 1.2f, 24, 24);
        glPopMatrix();
    }
    else
    {
        // MIDPOINT CIRCLE ALGORITHM
        int xc = (int)(x * 500);
        int yc = (int)(y * 500);
        int r = (int)(radius * 500);

        int px = 0;
        int py = r;
        int p = 1 - r;

        glColor3f(1.0f, 0.90f, 0.0f); // hot yellow

        while (px <= py)
        {
            drawPoint((xc + px) / 500.0f, (yc + py) / 500.0f);
            drawPoint((xc - px) / 500.0f, (yc + py) / 500.0f);
            drawPoint((xc + px) / 500.0f, (yc - py) / 500.0f);
            drawPoint((xc - px) / 500.0f, (yc - py) / 500.0f);

            drawPoint((xc + py) / 500.0f, (yc + px) / 500.0f);
            drawPoint((xc - py) / 500.0f, (yc + px) / 500.0f);
            drawPoint((xc + py) / 500.0f, (yc - px) / 500.0f);
            drawPoint((xc - py) / 500.0f, (yc - px) / 500.0f);

            if (p < 0)
                p += 2 * px + 3;
            else
            {
                p += 2 * (px - py) + 5;
                py--;
            }
            px++;
        }
    }
}

// ======================= BRICK GRID ==========================
BrickGrid::BrickGrid()
{
    reset();
}

void BrickGrid::reset()
{
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            bricks[i][j] = true;
}

void BrickGrid::brickPos(int i, int j, float &bx, float &by) const
{
    // i,j row, column of bricks
    // bx, by is actual screen position of bricks

    bx = startX + j * gapX;
    by = startY - i * gapY;
    // columns increase horizontally
    // rows increase downward
}

// bx, by bottom left corner position of brick

void BrickGrid::draw3DBrick(float bx, float by, float cR, float cG, float cB)
{
    glPushMatrix(); //saves the current matrix
    glTranslatef(bx + brickW / 2, by + brickH / 2, 0.0f); //moves to the brick's centre
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.3f); //spins it around a slightly tilted Y axis (angle, axis x, axis y, axis z)
    glScalef(brickW, brickH, 0.12f); //cube is originally 1x1x1, teslai scale garera rectangle banauney

    // ambient lighting color
    GLfloat amb[] = {cR * 0.7f, cG * 0.f, cB * 0.7f, 1.0f};
    // diffuse lighting
    GLfloat diff[] = {cR, cG, cB, 1.0f};
    // specular lighting
    GLfloat spec[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat shin = 20.0f; // shininess

    glMaterialfv(GL_FRONT, GL_AMBIENT, amb); //front face ma ambient properties
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMaterialf(GL_FRONT, GL_SHININESS, shin);
    glutSolidCube(1.0);
    glPopMatrix(); //restores the matrix
}

void BrickGrid::draw()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (!bricks[i][j]) //ball has broken brick
                continue;

            float bx, by;
            brickPos(i, j, bx, by);

             float rowColors[5][3] = {
                {1.0f, 0.08f, 0.58f},
                {1.0f, 0.42f, 0.0f},
                {0.67f, 1.0f, 0.0f},
                {0.0f, 0.90f, 1.0f},
                {0.75f, 0.0f, 1.0f}};
            float cR = rowColors[i][0];
            float cG = rowColors[i][1];
            float cB = rowColors[i][2];

            if (enable3D)
            {
                draw3DBrick(bx, by, cR, cG, cB);
            }
            else
            {
                glColor3f(cR, cG, cB);
                float x1 = bx;
                float y1 = by;
                float x2 = bx + brickW;
                float y2 = by + brickH;

                drawRectangle(x1, y1, x2, y2);
                glColor3f(cR * 0.85f, cG * 0.85f, cB * 0.85f);
                scanlineFill(bx, by, bx + brickW, by + brickH);
                glColor3f(1.0f, 1.0f, 1.0f);
                drawRectangle(bx, by, bx + brickW, by + brickH);
            }
        }
    }
}

bool BrickGrid::checkCollision(Ball &ball)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (!bricks[i][j])
                continue;

            float bx, by;
            brickPos(i, j, bx, by);

            if (ball.x > bx && ball.x < bx + brickW &&
                ball.y > by && ball.y < by + brickH)
            {
                bricks[i][j] = false;
                ball.dy *= -1;
                score += 10;
                return true;
            }
        }
    }
    return false;
}

bool BrickGrid::allCleared()
{
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (bricks[i][j])
                return false;
    return true;
}
