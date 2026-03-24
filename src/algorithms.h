#ifndef ALGORITHMS_H
#define ALGORITHMS_H

// ======================= RASTER ALGORITHMS ====================

void drawPoint(float x, float y);
void drawLineDDA(float x1, float y1, float x2, float y2);
void drawRectangle(float x1, float y1, float x2, float y2);
void scanlineFill(float x1, float y1, float x2, float y2);

#endif