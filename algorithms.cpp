#include "algorithms.h"
#include <GL/glut.h>
#include <cmath>

// ======================= RASTERIZE A POINT ====================
void drawPoint(float x, float y)
{
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

// ======================= DDA LINE ALGORITHM ===================
void drawLineDDA(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;

    int steps = (fabs(dx) > fabs(dy)) ? fabs(dx) * 500 : fabs(dy) * 500;
    // -1,1 xa window so, dx max=2 ie only 2 steps. so 500x gives 1000 sample points

    float xInc = dx / steps;
    float yInc = dy / steps;

    float x = x1;
    float y = y1;

    for (int i = 0; i < steps; i++)
    {
        drawPoint(x, y);
        x += xInc;
        y += yInc;
    }
}

// ======================= RECTANGLE OUTLINE (DDA) ==============
// top, right, bottom, left 
void drawRectangle(float x1, float y1, float x2, float y2)
{
    drawLineDDA(x1, y1, x2, y1);
    drawLineDDA(x2, y1, x2, y2);
    drawLineDDA(x2, y2, x1, y2);
    drawLineDDA(x1, y2, x1, y1);
}

// ======================= SCANLINE FILL ========================
// fills rectangle
void scanlineFill(float x1, float y1, float x2, float y2)
{
    for (float y = y1; y <= y2; y += 0.002f)
    {
        drawLineDDA(x1, y, x2, y);
    }
}