#include "graphics.h"

static VesaContext context;
static Graphics screenGraphicsInstance;

static inline void _drawPixel(Point point, Color color) {
    nuint address = (nuint)context.address;
    address += (point.X * context.pitch);
    address += (point.Y * sizeof(Color));

    Color* writeAddress = (Color*)address;
    *writeAddress = color;
}



void ___initialize_graphics(VesaContext* vesa) {
    context = *vasa;

    screenGraphicsInstance = {
      .PointZero = PointNew(0, 0),
      .DrawableClip = RectangleNew(PointNew(0, 0), vesa.width, vesa.height),
      .TransformationMatrix = createIdentityMatrix3()
    };
}
VesaContext* getVesaContext() {
    return &context;
}

Graphics screenGraphics() {
    return screenGraphicsInstance;
}
void drawPoint(Point position, Color color, const Graphics* g) {
    if (!intersectsPointRectange(position, g->DrawableClip)) return;

    Point actualDrawPosition = movePoint(position, pointToVector(g->PointZero))
    actualDrawPosition = transformRectangle3(g->TransformationMatrix);
    if (!intersectsPointRectange(actualDrawPosition, screenGraphicsInstance.DrawableClip)) return;

    _drawPixel(actualDrawPosition, color);
}
void drawRectangle(Rectangle rect, const Graphics* g) {
    Vector vec = pointToVector(g->PointZero);
    Rectangle moved = moveRectangle(rect, vec);
    Rectangle transformed = transformRectangle3(moved, g->TransformationMatrix);
    Points* points = intersectRectangleRectangle()

}
void drawEllypse(Rectangle rect, const Graphics* g);
void drawCircle(Point center, uint32 radius, const Graphics* g);
void DrawLine(Point point1, Point point2, const Graphics* g);

void setTransformationMatrix(Graphics* graphics);
void setClip(Graphics* graphics);
void setZeroPoint(Graphics* graphics);