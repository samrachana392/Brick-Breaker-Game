#ifndef OBJECTS_H
#define OBJECTS_H

#include "globals.h"

// ======================= PADDLE ==============================
class Paddle
{
public:
    float x     = 0.0f; //centre
    float width = 0.4f;

    void moveLeft(); //subtracts 0.025f from x
    void moveRight();
    void draw();
};

// Forward declaration so Ball can be used in BrickGrid
class Ball;

// ======================= BALL ================================
class Ball
{
public:
    float x      =  0.0f;
    float y      =  0.50f;
    float dx     =  0.012f;
    float dy     = -0.016f;
    float radius =  0.03f;

    void move();
    void resetPosition();
    void draw();
};

// ======================= BRICK GRID ==========================
class BrickGrid
{
public:
    bool  bricks[ROWS][COLS];
    float brickW =  0.200f;
    float brickH =  0.065f;
    float startX = -0.880f;
    float startY =  0.680f;
    float gapX   =  0.225f;
    float gapY   =  0.095f;

    BrickGrid();
    void reset();
    void brickPos(int i, int j, float &bx, float &by) const;
    void draw3DBrick(float bx, float by, float cR, float cG, float cB);
    void draw();
    bool checkCollision(Ball &ball);
    bool allCleared();
};

// ======================= GLOBAL OBJECT DECLARATIONS ==========
extern Paddle    paddle;
extern Ball      ball;
extern BrickGrid brickGrid;

#endif
