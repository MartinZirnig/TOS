#ifndef GRAPHICS_STRUCTURES_H
#define GRAPHICS_STRUCTURES_H

#include "std.h"

typedef struct
{
    int32 X;
    int32 Y;
} Point;

typedef struct 
{
    uint32 Count;
    Point Points[];
} Points;


typedef struct 
{
    int32 X;
    int32 Y;
} Vector;

typedef struct 
{
    Point Point;
    Vector Vector;
} Line;

typedef struct 
{
    Point Point1;
    Point Point2;
} LineSegment;


typedef struct 
{
    Point Edge1;
    Point Edge2;
    Point Edge3;
    Point Edge4;
} Rectangle;

typedef struct 
{
    Point Center;
    int32 X;
    int32 Y;
} Ellipse;

typedef struct  
{
    int32 A, B;
    int32 C, D;
} Matrix;

typedef struct {
    int32 A, B, C;
    int32 D, E, F;
} Matrix3;

typedef struct
{
    byte Alpha;
    byte Red;
    byte Green;
    byte Blue;
} Color;

typedef enum {
    MirrorAxeY = 1,
    MirrorAxeX = 2,
    MirrorAxeTL = 4,
    MirrorAxeTR = 8
} MirrorAxes;

Point PointNew(int32 x, int32 y);
Points* PointsNew(int32 count, ...);
Vector VectorNew(int32 x, int32 y);
Rectangle RectangleNew(Point edge1, Point edge2, Point edge3, Point edge4);
Rectangle RectangeLinearNew(int32 x, int32 y, int32 width, int32 height);
Ellipse CircleNew(Point center, int32 radius);
Ellipse EllipseNew(Point center, int32 x, int32 y);
Line LineNew(Point point, Vector vec);
LineSegment LineSegmentNew(Point start, Point end);
Matrix MatrixNew(int32 a, int32 b, int32 c, int32 d);
Matrix3 Matrix3New(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f);
Color ColorNew(byte red, byte green, byte blue, byte alpha);
Color ColorRGB(byte red, byte green, byte blue);


Vector pointToVector(Point point);
Point movePoint(Point point, Vector vector);
Point transformPoint(Point point, Matrix matrix);
Point transformPoint3(Point point, Matrix3 matrix);

Point vectorToPoint(Vector vector);
Vector transformVector(Vector vector, Matrix matrix);
Vector transformVector3(Vector vector, Matrix3 matrix);
int32 vectorLength(Vector vector);
Vector normalizeVector(Vector vector);
Vector subtractVectors(Vector left, Vector right);
Vector multiplyVector(Vector vector, int32 scalar);
int32 dotProduct(Vector left, Vector right);
Vector sumVectors(Vector base, Vector add);

Line moveLine(Line line, Vector vec);
Line transformLine(Line line, Matrix matrix);
Line transformLine3(Line line, Matrix3 matrix);

Line lineSegmentToLine(LineSegment segment);
LineSegment moveLineSegment(LineSegment segment, Vector vector);
LineSegment transformLineSegment(LineSegment segment, Matrix matrix);
LineSegment transformLineSegment3(LineSegment segment, Matrix3 matrix);
int32 lineSegmentLength(LineSegment segment);
int32 lineSegmentLengthSquared(LineSegment segment);
Vector lineSegmentVector(LineSegment segment);
Point lineSegmentCenter(LineSegment segment);

Ellipse moveEllipse(Ellipse ellipse, Vector vec);
Ellipse transformEllipse(Ellipse ellipse, Matrix matrix);
Ellipse transformEllipse3(Ellipse ellipse, Matrix3 matrix);

LineSegment rectangleTop(Rectangle rect);
LineSegment rectangleBottom(Rectangle rect);
LineSegment rectangleLeft(Rectangle rect);
LineSegment rectangleRight(Rectangle rect);
Rectangle moveRectangle(Rectangle rect, Vector vector);
Rectangle transformRectangle(Rectangle rect, Matrix matrix);
Rectangle transformRectangle3(Rectangle rect, Matrix3 matrix);

int32 determinantMatrix(Matrix matrix);
int32 determinantMatrix3(Matrix3 matrix);

bool equalsPoints(Point p1, Point p2);
bool equalsVectors(Vector v1, Vector v2);
bool equalsLines(Line l1, Line l2);
bool equalsRectangles(Rectangle r1, Rectangle r2);
bool equalsEllipses(Ellipse c1, Ellipse c2);
bool equalsMatrix(Matrix m1, Matrix m2);
bool equalsMatrix3(Matrix3 m1, Matrix3 m2);

bool intersectsPointLine(Point point, Line line);
bool intersectsPointRectange(Point point, Rectangle rect);
bool intersectsPointEllipse(Point point, Ellipse ellipse);
bool intersectsLineRectangle(Line line, Rectangle rectangle);
bool intersectsLineEllipse(Line line, Ellipse ellipse);
bool intersectsEllipseRectangle(Ellipse ellipse, Rectangle rectangle);
bool intersectsLineLine(Line l1, Line l2);
bool intersectsRectangleRectangle(Rectangle r1, Rectangle r2);
bool intersectsEllipseEllipse(Ellipse e1, Ellipse e2);
bool intersectsLineSegmentLineSegment(LineSegment s1, LineSegment s2);
bool intersectsLineSegmentRectangle(LineSegment segment, Rectangle rect);
bool intersectsLineSegmentEllipse(LineSegment segment, Ellipse ellipse);


Points* intersectLineRectangle(Line line, Rectangle rectangle);
Points* intersectLineEllipse(Line line, Ellipse ellipse);
Points* intersectEllipseRectangle(Ellipse ellipse, Rectangle rectangle);
Points* intersectRectangleRectangle(Rectangle r1, Rectangle r2);
Points* intersectEllipseEllipse(Ellipse e1, Ellipse e2);
Points* intersectLineSegmentRectangle(LineSegment segment, Rectangle rect);
Points* intersectLineSegmentEllipse(LineSegment segment, Ellipse ellipse);
Point intersectLineSegmentLine(LineSegment segment, Line line);
Point intersectLineSegmentLineSegment(LineSegment segment1, LineSegment segment2);
Point intersectLineLine(Line line1, Line line2);


Points* andLineRectangle(Line line, Rectangle rect);
Points* orLineRectangle(Line line, Rectangle rect);

Points* andLineEllipse(Line line, Ellipse ellipse);
Points* orLineEllipse(Line line, Ellipse ellipse);

Points* andLineSegmentRectangle(LineSegment segment, Rectangle rect);
Points* orLineSegmentRectangle(LineSegment segment, Rectangle rect);

Points* andLineSegmentEllipse(LineSegment segment, Ellipse ellipse);
Points* orLineSegmentEllipse(LineSegment segment, Ellipse ellipse);

Points* andRectangleEllipse(Rectangle rect, Ellipse ellipse);
Points* orRectangleEllipse(Rectangle rect, Ellipse ellipse);

Points* andEllipseEllipse(Ellipse ellipse1, Ellipse ellipse2);
Points* orEllipseEllipse(Ellipse ellipse1, Ellipse ellipse2);

Matrix createIdentityMatrix();
Matrix createRotationMatrix(int32 degrees);
Matrix createScaleMatrix(int32 scaleX, int32 scaleY);
Matrix createShearMatrix(int32 shearX, int32 shearY);
Matrix createMirrorMatrix(MirrorAxes axe);
Matrix multiplyMatrix(Matrix left, Matrix right);
Matrix invertMatrix(Matrix matrix);
Matrix transposeMatrix(Matrix matrix);

Matrix3 createIdentityMatrix3();
Matrix3 createTranslationMatrix3(Vector translation);
Matrix3 createRotationMatrix3(int32 degrees);
Matrix3 createScaleMatrix3(int32 scaleX, int32 scaleY);
Matrix3 createShearMatrix3(int32 shearX, int32 shearY);
Matrix3 createMirrorMatrix3(MirrorAxes axe);
Matrix3 multiplyMatrix3(Matrix3 left, Matrix3 right);
Matrix3 invertMatrix3(Matrix3 matrix);
Matrix3 transposeMatrix3(Matrix3 matrix);

#endif