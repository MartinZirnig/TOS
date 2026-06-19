#include "graphics_structures.h"
#include <stdarg.h>  
Point PointNew(int32 x, int32 y) {
    return (Point){ .X = x, .Y = y };
}

Points* PointsNew(int32 count, ...) {
    Points* pts = (Points*)malloc(sizeof(Points) + (uint32)count * sizeof(Point));
    if (!pts) return null;

    pts->Count = (uint32)count;

    va_list args;
    va_start(args, count);
    for (int32 i = 0; i < count; i++) {
        pts->Points[i] = va_arg(args, Point);
    }
    va_end(args);

    return pts;
}

Vector VectorNew(int32 x, int32 y) {
    return (Vector){ .X = x, .Y = y };
}

Rectangle RectangleNew(Point edge1, Point edge2, Point edge3, Point edge4) {
    return (Rectangle){
        .Edge1 = edge1,
        .Edge2 = edge2,
        .Edge3 = edge3,
        .Edge4 = edge4
    };
}

Rectangle RectangeLinearNew(int32 x, int32 y, int32 width, int32 height) {
    return (Rectangle){
        .Edge1 = PointNew(x,         y),
        .Edge2 = PointNew(x + width, y),
        .Edge3 = PointNew(x + width, y + height),
        .Edge4 = PointNew(x,         y + height)
    };
}

Ellipse EllipseNew(Point center, int32 x, int32 y) {
    return (Ellipse){ .Center = center, .X = x, .Y = y };
}

Ellipse CircleNew(Point center, int32 radius) {
    return (Ellipse){ .Center = center, .X = radius, .Y = radius };
}


Line LineNew(Point point, Vector vec) {
    return (Line){ .Point = point, .Vector = vec };
}

LineSegment LineSegmentNew(Point start, Point end) {
    return (LineSegment){ .Point1 = start, .Point2 = end };
}

Matrix MatrixNew(int32 a, int32 b, int32 c, int32 d) {
    return (Matrix){ .A = a, .B = b, .C = c, .D = d };
}

Matrix3 Matrix3New(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f) {
    return (Matrix3){ .A = a, .B = b, .C = c, .D = d, .E = e, .F = f };
}

Color ColorNew(byte red, byte green, byte blue, byte alpha) {
    return (Color){ .Alpha = alpha, .Red = red, .Green = green, .Blue = blue };
}

Color ColorRGB(byte red, byte green, byte blue) {
    return (Color){ .Alpha = 0xFF, .Red = red, .Green = green, .Blue = blue };
}