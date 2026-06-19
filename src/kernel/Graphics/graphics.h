#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "types.h"
#include "graphics_structures.h"
#include "vesa.h"

typedef struct
{
    Point PointZero;
    Rectangle DrawableClip;
    Matrix3 TransformationMatrix;
} Graphics;

void ___initialize_graphics(VesaContext* vesa);
VesaContext* getVesaContext();

Graphics screenGraphics();
void drawPoint(Point position, Color color, const Graphics* g);
void drawRectangle(Rectangle rect, const Graphics* g);
void drawEllypse(Rectangle rect, const Graphics* g);
void drawCircle(Point center, uint32 radius, const Graphics* g);
void DrawLine(Point point1, Point point2, const Graphics* g);

void setTransformationMatrix(Graphics* graphics);
void setClip(Graphics* graphics);
void setZeroPoint(Graphics* graphics);

#endif