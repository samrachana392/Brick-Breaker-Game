#ifndef GLOBALS_H
#define GLOBALS_H

#include <GL/glut.h>
#include <string>

// ======================= CONSTANTS ============================
const int WINDOW_W = 900;
const int WINDOW_H = 600;
const int ROWS     = 5;
const int COLS     = 8;
const int MAX_LEVEL = 3;

// ======================= GAME STATE ==========================
enum GameState
{
    MENU,
    COUNTDOWN,
    PLAYING,
    PAUSED,
    LIFE_LOST,
    GAME_OVER,
    WIN_SCREEN
};

// Declared in main.cpp, accessible everywhere via extern
extern GameState    gameState;
extern int          score;
extern int          lives;
extern int          livesLost;
extern int          level;
extern std::string  difficulty;

extern int   lifeLostMessageTime;
extern int   countdownStartTime;
extern int   countdownNumber;

extern bool  enable3D;
extern float rotationAngle;

extern bool  keyLeft;
extern bool  keyRight;

#endif