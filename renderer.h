#ifndef RENDERER_H
#define RENDERER_H

#include <string>

// ======================= LIGHTING ============================
void setupLighting();
void disableLighting();

// ======================= TEXT ================================
void drawText(float x, float y, const std::string &s,
              float r = 1.0f, float g = 1.0f, float b = 1.0f);
void drawTextLarge(float x, float y, const std::string &s,
                   float r = 1.0f, float g = 1.0f, float b = 1.0f);

// ======================= HUD =================================
void drawHUD();

// ======================= PROJECTION ==========================
void setProjection();
void force2D();

// ======================= DISPLAY =============================
void display();

#endif
