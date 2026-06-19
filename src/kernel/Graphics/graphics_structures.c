#include "graphics_structures.h"
#include "math.h"

#ifdef E
#undef E
#endif

#define DEG_TO_RAD(deg) ((double)(deg) * PI / 180.0)

Point PointNew(int32 x, int32 y) {
    Point p = { x, y };
    return p;
}

Points PointsNew(int32 count, ...) {
    Points pts;
    pts.Count = count;
    return pts;
}

Vector VectorNew(int32 x, int32 y) {
    Vector v = { x, y };
    return v;
}

Rectangle RectangleNew(Point edge1, Point edge2, Point edge3, Point edge4) {
    Rectangle r = { edge1, edge2, edge3, edge4 };
    return r;
}

Rectangle RectangeLinearNew(int32 x, int32 y, int32 width, int32 height) {
    Rectangle r;
    r.Edge1 = PointNew(x, y);
    r.Edge2 = PointNew(x + width, y);
    r.Edge3 = PointNew(x + width, y + height);
    r.Edge4 = PointNew(x, y + height);
    return r;
}

Ellipse CircleNew(Point center, int32 radius) {
    Ellipse e = { center, radius, radius };
    return e;
}

Ellipse EllipseNew(Point center, int32 x, int32 y) {
    Ellipse e = { center, x, y };
    return e;
}

Line LineNew(Point point, Vector vec) {
    Line l = { point, vec };
    return l;
}

LineSegment LineSegmentNew(Point start, Point end) {
    LineSegment ls = { start, end };
    return ls;
}

Matrix MatrixNew(int32 a, int32 b, int32 c, int32 d) {
    Matrix m = { a, b, c, d };
    return m;
}

Matrix3 Matrix3New(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f) {
    Matrix3 m = { a, b, c, d, e, f };
    return m;
}

Color ColorNew(byte red, byte green, byte blue, byte alpha) {
    Color c = { alpha, red, green, blue };
    return c;
}

Color ColorRGB(byte red, byte green, byte blue) {
    Color c = { 255, red, green, blue };
    return c;
}

Vector pointToVector(Point point) {
    return VectorNew(point.X, point.Y);
}

Point movePoint(Point point, Vector vector) {
    return PointNew(point.X + vector.X, point.Y + vector.Y);
}

Point transformPoint(Point point, Matrix matrix) {
    return PointNew(
        point.X * matrix.A + point.Y * matrix.C,
        point.X * matrix.B + point.Y * matrix.D
    );
}

Point transformPoint3(Point point, Matrix3 m) {
    return PointNew(
        point.X * m.A + point.Y * m.C + m.D,
        point.X * m.B + point.Y * m.E + m.F
    );
}

Point vectorToPoint(Vector vector) {
    return PointNew(vector.X, vector.Y);
}

Vector transformVector(Vector vector, Matrix matrix) {
    return VectorNew(
        vector.X * matrix.A + vector.Y * matrix.C,
        vector.X * matrix.B + vector.Y * matrix.D
    );
}

Vector transformVector3(Vector vector, Matrix3 m) {
    return VectorNew(
        vector.X * m.A + vector.Y * m.C,
        vector.X * m.B + vector.Y * m.E
    );
}

int32 vectorLength(Vector vector) {
    double lengthSq = (double)vector.X * vector.X + (double)vector.Y * vector.Y;
    return (int32)sqrt(lengthSq);
}

Vector normalizeVector(Vector vector) {
    double len = sqrt((double)vector.X * vector.X + (double)vector.Y * vector.Y);
    if (len == 0.0) return VectorNew(0, 0);
    return VectorNew((int32)(vector.X / len), (int32)(vector.Y / len));
}

Vector subtractVectors(Vector left, Vector right) {
    return VectorNew(left.X - right.X, left.Y - right.Y);
}

Vector multiplyVector(Vector vector, int32 scalar) {
    return VectorNew(vector.X * scalar, vector.Y * scalar);
}

int32 dotProduct(Vector left, Vector right) {
    return left.X * right.X + left.Y * right.Y;
}

Vector sumVectors(Vector base, Vector add) {
    return VectorNew(base.X + add.X, base.Y + add.Y);
}

Line moveLine(Line line, Vector vec) {
    return LineNew(movePoint(line.Point, vec), line.Vector);
}

Line transformLine(Line line, Matrix matrix) {
    return LineNew(transformPoint(line.Point, matrix), transformVector(line.Vector, matrix));
}

Line transformLine3(Line line, Matrix3 matrix) {
    return LineNew(transformPoint3(line.Point, matrix), transformVector3(line.Vector, matrix));
}

Line lineSegmentToLine(LineSegment segment) {
    Vector v = VectorNew(segment.Point2.X - segment.Point1.X, segment.Point2.Y - segment.Point1.Y);
    return LineNew(segment.Point1, v);
}

LineSegment moveLineSegment(LineSegment segment, Vector vector) {
    return LineSegmentNew(movePoint(segment.Point1, vector), movePoint(segment.Point2, vector));
}

LineSegment transformLineSegment(LineSegment segment, Matrix matrix) {
    return LineSegmentNew(transformPoint(segment.Point1, matrix), transformPoint(segment.Point2, matrix));
}

LineSegment transformLineSegment3(LineSegment segment, Matrix3 matrix) {
    return LineSegmentNew(transformPoint3(segment.Point1, matrix), transformPoint3(segment.Point2, matrix));
}

int32 lineSegmentLength(LineSegment segment) {
    double dx = (double)segment.Point2.X - segment.Point1.X;
    double dy = (double)segment.Point2.Y - segment.Point1.Y;
    return (int32)sqrt(dx * dx + dy * dy);
}

int32 lineSegmentLengthSquared(LineSegment segment) {
    int32 dx = segment.Point2.X - segment.Point1.X;
    int32 dy = segment.Point2.Y - segment.Point1.Y;
    return dx * dx + dy * dy;
}

Vector lineSegmentVector(LineSegment segment) {
    return VectorNew(segment.Point2.X - segment.Point1.X, segment.Point2.Y - segment.Point1.Y);
}

Point lineSegmentCenter(LineSegment segment) {
    return PointNew((segment.Point1.X + segment.Point2.X) / 2, (segment.Point1.Y + segment.Point2.Y) / 2);
}


Ellipse moveEllipse(Ellipse ellipse, Vector vec) {
    return EllipseNew(movePoint(ellipse.Center, vec), ellipse.X, ellipse.Y);
}

Ellipse transformEllipse(Ellipse ellipse, Matrix matrix) {
    Point newCenter = transformPoint(ellipse.Center, matrix);
    double scaleX = sqrt((double)matrix.A * matrix.A + (double)matrix.B * matrix.B);
    double scaleY = sqrt((double)matrix.C * matrix.C + (double)matrix.D * matrix.D);
    return EllipseNew(newCenter, (int32)(ellipse.X * scaleX), (int32)(ellipse.Y * scaleY));
}

Ellipse transformEllipse3(Ellipse ellipse, Matrix3 matrix) {
    Point newCenter = transformPoint3(ellipse.Center, matrix);
    double scaleX = sqrt((double)matrix.A * matrix.A + (double)matrix.B * matrix.B);
    double scaleY = sqrt((double)matrix.C * matrix.C + (double)matrix.E * matrix.E);
    return EllipseNew(newCenter, (int32)(ellipse.X * scaleX), (int32)(ellipse.Y * scaleY));
}



LineSegment rectangleTop(Rectangle rect)    { return LineSegmentNew(rect.Edge1, rect.Edge2); }
LineSegment rectangleRight(Rectangle rect)  { return LineSegmentNew(rect.Edge2, rect.Edge3); }
LineSegment rectangleBottom(Rectangle rect) { return LineSegmentNew(rect.Edge3, rect.Edge4); }
LineSegment rectangleLeft(Rectangle rect)   { return LineSegmentNew(rect.Edge4, rect.Edge1); }

Rectangle moveRectangle(Rectangle rect, Vector vector) {
    return RectangleNew(
        movePoint(rect.Edge1, vector), movePoint(rect.Edge2, vector),
        movePoint(rect.Edge3, vector), movePoint(rect.Edge4, vector)
    );
}

Rectangle transformRectangle(Rectangle rect, Matrix matrix) {
    return RectangleNew(
        transformPoint(rect.Edge1, matrix), transformPoint(rect.Edge2, matrix),
        transformPoint(rect.Edge3, matrix), transformPoint(rect.Edge4, matrix)
    );
}

Rectangle transformRectangle3(Rectangle rect, Matrix3 matrix) {
    return RectangleNew(
        transformPoint3(rect.Edge1, matrix), transformPoint3(rect.Edge2, matrix),
        transformPoint3(rect.Edge3, matrix), transformPoint3(rect.Edge4, matrix)
    );
}


int32 determinantMatrix(Matrix matrix) {
    return matrix.A * matrix.D - matrix.B * matrix.C;
}

int32 determinantMatrix3(Matrix3 matrix) {
    return matrix.A * matrix.E - matrix.B * matrix.C;
}

bool equalsPoints(Point p1, Point p2) { return p1.X == p2.X and p1.Y == p2.Y; }
bool equalsVectors(Vector v1, Vector v2) { return v1.X == v2.X and v1.Y == v2.Y; }
bool equalsLines(Line l1, Line l2) { return equalsPoints(l1.Point, l2.Point) and equalsVectors(l1.Vector, l2.Vector); }
bool equalsRectangles(Rectangle r1, Rectangle r2) {
    return equalsPoints(r1.Edge1, r2.Edge1) and equalsPoints(r1.Edge2, r2.Edge2) and
           equalsPoints(r1.Edge3, r2.Edge3) and equalsPoints(r1.Edge4, r2.Edge4);
}
bool equalsEllipses(Ellipse c1, Ellipse c2) { return equalsPoints(c1.Center, c2.Center) and c1.X == c2.X and c1.Y == c2.Y; }
bool equalsMatrix(Matrix m1, Matrix m2) { return m1.A == m2.A and m1.B == m2.B and m1.C == m2.C and m1.D == m2.D; }
bool equalsMatrix3(Matrix3 m1, Matrix3 m2) {
    return m1.A == m2.A and m1.B == m2.B and m1.C == m2.C and m1.D == m2.D and m1.E == m2.E and m1.F == m2.F;
}



bool intersectsPointLine(Point point, Line line) {
    return (point.X - line.Point.X) * line.Vector.Y - (point.Y - line.Point.Y) * line.Vector.X == 0;
}

static int32 triangleArea(Point a, Point b, Point c) {
    return abs(a.X * (b.Y - c.Y) + b.X * (c.Y - a.Y) + c.X * (a.Y - b.Y));
}

bool intersectsPointRectange(Point point, Rectangle rect) {
    int32 rectArea = triangleArea(rect.Edge1, rect.Edge2, rect.Edge3) + triangleArea(rect.Edge1, rect.Edge3, rect.Edge4);
    int32 pArea = triangleArea(point, rect.Edge1, rect.Edge2) + triangleArea(point, rect.Edge2, rect.Edge3) +
                  triangleArea(point, rect.Edge3, rect.Edge4) + triangleArea(point, rect.Edge4, rect.Edge1);
    return pArea <= rectArea;
}

bool intersectsPointEllipse(Point point, Ellipse ellipse) {
    if (ellipse.X == 0 or ellipse.Y == 0) return false;
    double dx = (double)point.X - ellipse.Center.X;
    double dy = (double)point.Y - ellipse.Center.Y;
    return ((dx * dx) / (ellipse.X * ellipse.X) + (dy * dy) / (ellipse.Y * ellipse.Y)) <= 1.0;
}

bool intersectsLineLine(Line l1, Line l2) {
    return (l1.Vector.X * l2.Vector.Y - l1.Vector.Y * l2.Vector.X) != 0;
}

bool intersectsLineSegmentLineSegment(LineSegment s1, LineSegment s2) {
    int32 d = (s1.Point2.X - s1.Point1.X) * (s2.Point2.Y - s2.Point1.Y) - (s1.Point2.Y - s1.Point1.Y) * (s2.Point2.X - s2.Point1.X);
    if (d == 0) return false;
    int32 u = ((s2.Point1.X - s1.Point1.X) * (s2.Point2.Y - s2.Point1.Y) - (s2.Point1.Y - s1.Point1.Y) * (s2.Point2.X - s2.Point1.X));
    int32 v = ((s2.Point1.X - s1.Point1.X) * (s1.Point2.Y - s1.Point1.Y) - (s2.Point1.Y - s1.Point1.Y) * (s1.Point2.X - s1.Point1.X));
    
    if (d > 0) {
        return (u >= 0 and u <= d and v >= 0 and v <= d);
    } else {
        return (u <= 0 and u >= d and v <= 0 and v >= d);
    }
}

bool intersectsLineRectangle(Line line, Rectangle rectangle) {
    return intersectsPointRectange(line.Point, rectangle) or
           intersectsLineSegmentLineSegment(rectangleTop(rectangle), rectangleTop(rectangle)) or
           intersectsLineSegmentLineSegment(rectangleBottom(rectangle), rectangleBottom(rectangle));
}

bool intersectsLineEllipse(Line line, Ellipse ellipse) {
    double a = ((double)line.Vector.X * line.Vector.X) / (ellipse.X * ellipse.X) + ((double)line.Vector.Y * line.Vector.Y) / (ellipse.Y * ellipse.Y);
    double x0 = (double)line.Point.X - ellipse.Center.X;
    double y0 = (double)line.Point.Y - ellipse.Center.Y;
    double b = 2.0 * ((x0 * line.Vector.X) / (ellipse.X * ellipse.X) + (y0 * line.Vector.Y) / (ellipse.Y * ellipse.Y));
    double c = (x0 * x0) / (ellipse.X * ellipse.X) + (y0 * y0) / (ellipse.Y * ellipse.Y) - 1.0;
    return (b * b - 4.0 * a * c) >= 0.0;
}

bool intersectsLineSegmentRectangle(LineSegment segment, Rectangle rect) {
    if (intersectsPointRectange(segment.Point1, rect) or intersectsPointRectange(segment.Point2, rect)) return true;
    return intersectsLineSegmentLineSegment(segment, rectangleTop(rect)) or
           intersectsLineSegmentLineSegment(segment, rectangleRight(rect)) or
           intersectsLineSegmentLineSegment(segment, rectangleBottom(rect)) or
           intersectsLineSegmentLineSegment(segment, rectangleLeft(rect));
}

bool intersectsLineSegmentEllipse(LineSegment segment, Ellipse ellipse) {
    if (intersectsPointEllipse(segment.Point1, ellipse) or intersectsPointEllipse(segment.Point2, ellipse)) return true;
    double a = ((double)lineSegmentVector(segment).X * lineSegmentVector(segment).X) / (ellipse.X * ellipse.X) + 
               ((double)lineSegmentVector(segment).Y * lineSegmentVector(segment).Y) / (ellipse.Y * ellipse.Y);
    double x0 = (double)segment.Point1.X - ellipse.Center.X;
    double y0 = (double)segment.Point1.Y - ellipse.Center.Y;
    double b = 2.0 * ((x0 * lineSegmentVector(segment).X) / (ellipse.X * ellipse.X) + (y0 * lineSegmentVector(segment).Y) / (ellipse.Y * ellipse.Y));
    double c = (x0 * x0) / (ellipse.X * ellipse.X) + (y0 * y0) / (ellipse.Y * ellipse.Y) - 1.0;
    double disc = b * b - 4.0 * a * c;
    if (disc < 0.0) return false;
    double t1 = (-b - sqrt(disc)) / (2.0 * a);
    double t2 = (-b + sqrt(disc)) / (2.0 * a);
    return (t1 >= 0.0 and t1 <= 1.0) or (t2 >= 0.0 and t2 <= 1.0);
}

bool intersectsRectangleRectangle(Rectangle r1, Rectangle r2) {
    return intersectsLineSegmentRectangle(rectangleTop(r1), r2) or
           intersectsLineSegmentRectangle(rectangleBottom(r1), r2) or
           intersectsPointRectange(r1.Edge1, r2);
}

bool intersectsEllipseRectangle(Ellipse ellipse, Rectangle rectangle) {
    if (intersectsPointEllipse(rectangle.Edge1, ellipse) or intersectsPointRectange(ellipse.Center, rectangle)) return true;
    return intersectsLineSegmentEllipse(rectangleTop(rectangle), ellipse) or
           intersectsLineSegmentEllipse(rectangleRight(rectangle), ellipse);
}

bool intersectsEllipseEllipse(Ellipse e1, Ellipse e2) {
    int32 maxR1 = max(e1.X, e1.Y);
    int32 maxR2 = max(e2.X, e2.Y);
    int32 dx = e1.Center.X - e2.Center.X;
    int32 dy = e1.Center.Y - e2.Center.Y;
    return ((int64)dx * dx + (int64)dy * dy) <= ((int64)(maxR1 + maxR2) * (maxR1 + maxR2));
}


Point intersectLineLine(Line line1, Line line2) {
    int32 denom = line1.Vector.X * line2.Vector.Y - line1.Vector.Y * line2.Vector.X;
    if (denom == 0) return PointNew(0, 0);
    int32 num = ((line2.Point.X - line1.Point.X) * line2.Vector.Y - (line2.Point.Y - line1.Point.Y) * line2.Vector.X);
    int32 t = num / denom;
    return PointNew(line1.Point.X + t * line1.Vector.X, line1.Point.Y + t * line1.Vector.Y);
}

Point intersectLineSegmentLine(LineSegment segment, Line line) {
    return intersectLineLine(lineSegmentToLine(segment), line);
}

Point intersectLineSegmentLineSegment(LineSegment segment1, LineSegment segment2) {
    return intersectLineLine(lineSegmentToLine(segment1), lineSegmentToLine(segment2));
}


static Points* allocatePoints(int32 count) {
    if (count <= 0) {
        Points* p = (Points*)malloc(sizeof(Points));
        if (p) p->Count = 0;
        return p;
    }

    Points* p = (Points*)malloc(sizeof(Points) + (count * sizeof(Point)));
    if (p) {
        p->Count = count;
    }
    return p;
}


Points* intersectLineRectangle(Line line, Rectangle rectangle) {
    LineSegment sides[4] = {
        rectangleTop(rectangle),
        rectangleRight(rectangle),
        rectangleBottom(rectangle),
        rectangleLeft(rectangle)
    };

    Point temp[4];
    int32 found = 0;

    for (int i = 0; i < 4; i++) {
        if (intersectsLineLine(lineSegmentToLine(sides[i]), line)) {
            Point pt = intersectLineSegmentLine(sides[i], line);
            
            bool duplicate = false;
            for (int j = 0; j < found; j++) {
                if (equalsPoints(temp[j], pt)) {
                    duplicate = true;
                    break;
                }
            }
            if (!duplicate && found < 4) {
                temp[found++] = pt;
            }
        }
    }

    Points* pts = allocatePoints(found);
    if (!pts) return NULL;

    for (int i = 0; i < found; i++) {
        pts->Points[i] = temp[i];
    }

    return pts;
}

Points* intersectLineEllipse(Line line, Ellipse ellipse) {
    double a = ((double)line.Vector.X * line.Vector.X) / (ellipse.X * ellipse.X) + 
               ((double)line.Vector.Y * line.Vector.Y) / (ellipse.Y * ellipse.Y);
    
    double x0 = (double)line.Point.X - ellipse.Center.X;
    double y0 = (double)line.Point.Y - ellipse.Center.Y;
    
    double b = 2.0 * ((x0 * line.Vector.X) / (ellipse.X * ellipse.X) + 
                      (y0 * line.Vector.Y) / (ellipse.Y * ellipse.Y));
    
    double c = (x0 * x0) / (ellipse.X * ellipse.X) + 
               (y0 * y0) / (ellipse.Y * ellipse.Y) - 1.0;

    double disc = b * b - 4.0 * a * c;

    if (disc < 0.0) {
        return allocatePoints(0);
    }

    int32 found = (disc == 0.0) ? 1 : 2;
    Points* pts = allocatePoints(found);
    if (!pts) return NULL;

    if (found == 1) {
        double t = -b / (2.0 * a);
        pts->Points[0] = PointNew(line.Point.X + (int32)(t * line.Vector.X), 
                                 line.Point.Y + (int32)(t * line.Vector.Y));
    } else {
        double t1 = (-b - sqrt(disc)) / (2.0 * a);
        double t2 = (-b + sqrt(disc)) / (2.0 * a);
        pts->Points[0] = PointNew(line.Point.X + (int32)(t1 * line.Vector.X), 
                                 line.Point.Y + (int32)(t1 * line.Vector.Y));
        pts->Points[1] = PointNew(line.Point.X + (int32)(t2 * line.Vector.X), 
                                 line.Point.Y + (int32)(t2 * line.Vector.Y));
    }

    return pts;
}

Points* intersectLineSegmentRectangle(LineSegment segment, Rectangle rect) {
    Line line = lineSegmentToLine(segment);
    Points* lineIntersections = intersectLineRectangle(line, rect);
    if (!lineIntersections) return NULL;
    
    int32 minX = min(segment.Point1.X, segment.Point2.X);
    int32 maxX = max(segment.Point1.X, segment.Point2.X);
    int32 minY = min(segment.Point1.Y, segment.Point2.Y);
    int32 maxY = max(segment.Point1.Y, segment.Point2.Y);

    int32 validCount = 0;
    Point temp[4];

    for (int i = 0; i < lineIntersections->Count; i++) {
        Point p = lineIntersections->Points[i];
        if (p.X >= minX && p.X <= maxX && p.Y >= minY && p.Y <= maxY) {
            temp[validCount++] = p;
        }
    }

    free(lineIntersections);

    Points* pts = allocatePoints(validCount);
    if (!pts) return NULL;
    for (int i = 0; i < validCount; i++) pts->Points[i] = temp[i];

    return pts;
}

Points* intersectLineSegmentEllipse(LineSegment segment, Ellipse ellipse) {
    Line line = lineSegmentToLine(segment);
    Points* lineIntersections = intersectLineEllipse(line, ellipse);
    if (!lineIntersections) return NULL;

    int32 minX = min(segment.Point1.X, segment.Point2.X);
    int32 maxX = max(segment.Point1.X, segment.Point2.X);
    int32 minY = min(segment.Point1.Y, segment.Point2.Y);
    int32 maxY = max(segment.Point1.Y, segment.Point2.Y);

    int32 validCount = 0;
    Point temp[2];

    for (int i = 0; i < lineIntersections->Count; i++) {
        Point p = lineIntersections->Points[i];
        if (p.X >= minX && p.X <= maxX && p.Y >= minY && p.Y <= maxY) {
            temp[validCount++] = p;
        }
    }

    free(lineIntersections);

    Points* pts = allocatePoints(validCount);
    if (!pts) return NULL;
    for (int i = 0; i < validCount; i++) pts->Points[i] = temp[i];

    return pts;
}

Points* intersectEllipseRectangle(Ellipse ellipse, Rectangle rectangle) {
    LineSegment sides[4] = {
        rectangleTop(rectangle),
        rectangleRight(rectangle),
        rectangleBottom(rectangle),
        rectangleLeft(rectangle)
    };

    Point temp[8];
    int32 found = 0;

    for (int i = 0; i < 4; i++) {
        Points* sideIntersects = intersectLineSegmentEllipse(sides[i], ellipse);
        if (!sideIntersects) continue;

        for (int j = 0; j < sideIntersects->Count; j++) {
            if (found < 8) {
                temp[found++] = sideIntersects->Points[j];
            }
        }
        free(sideIntersects);
    }

    Points* pts = allocatePoints(found);
    if (!pts) return NULL;
    for (int i = 0; i < found; i++) pts->Points[i] = temp[i];

    return pts;
}

Points* intersectRectangleRectangle(Rectangle r1, Rectangle r2) {
    LineSegment sides[4] = {
        rectangleTop(r1),
        rectangleRight(r1),
        rectangleBottom(r1),
        rectangleLeft(r1)
    };

    Point temp[8];
    int32 found = 0;

    for (int i = 0; i < 4; i++) {
        Points* intersects = intersectLineSegmentRectangle(sides[i], r2);
        if (!intersects) continue;

        for (int j = 0; j < intersects->Count; j++) {
            bool duplicate = false;
            for (int k = 0; k < found; k++) {
                if (equalsPoints(temp[k], intersects->Points[j])) {
                    duplicate = true;
                    break;
                }
            }
            if (!duplicate && found < 8) {
                temp[found++] = intersects->Points[j];
            }
        }
        free(intersects);
    }

    Points* pts = allocatePoints(found);
    if (!pts) return NULL;
    for (int i = 0; i < found; i++) pts->Points[i] = temp[i];

    return pts;
}

Points* intersectEllipseEllipse(Ellipse e1, Ellipse e2) {
    if (e1.X == e1.Y && e2.X == e2.Y) {
        double r0 = e1.X;
        double r1 = e2.X;
        double d = sqrt((double)(e2.Center.X - e1.Center.X) * (e2.Center.X - e1.Center.X) + 
                        (double)(e2.Center.Y - e1.Center.Y) * (e2.Center.Y - e1.Center.Y));

        if (d > r0 + r1 || d < abs(r0 - r1) || d == 0.0) {
            return allocatePoints(0);
        }

        double a = (r0 * r0 - r1 * r1 + d * d) / (2.0 * d);
        double h = sqrt(r0 * r0 - a * a);

        double x2 = e1.Center.X + a * (e2.Center.X - e1.Center.X) / d;
        double y2 = e1.Center.Y + a * (e2.Center.Y - e1.Center.Y) / d;

        if (h == 0.0) {
            Points* pts = allocatePoints(1);
            if (!pts) return NULL;
            pts->Points[0] = PointNew((int32)x2, (int32)y2);
            return pts;
        } else {
            Points* pts = allocatePoints(2);
            if (!pts) return NULL;
            pts->Points[0] = PointNew((int32)(x2 + h * (e2.Center.Y - e1.Center.Y) / d),
                                     (int32)(y2 - h * (e2.Center.X - e1.Center.X) / d));
            pts->Points[1] = PointNew((int32)(x2 - h * (e2.Center.Y - e1.Center.Y) / d),
                                     (int32)(y2 + h * (e2.Center.X - e1.Center.X) / d));
            return pts;
        }
    }

    return allocatePoints(0);
}


Matrix createIdentityMatrix() {
    return MatrixNew(1, 0, 0, 1);
}

Matrix createRotationMatrix(int32 degrees) {
    double rad = DEG_TO_RAD(degrees);
    return MatrixNew((int32)cos(rad), (int32)-sin(rad), (int32)sin(rad), (int32)cos(rad));
}

Matrix createScaleMatrix(int32 scaleX, int32 scaleY) {
    return MatrixNew(scaleX, 0, 0, scaleY);
}

Matrix createShearMatrix(int32 shearX, int32 shearY) {
    return MatrixNew(1, shearX, shearY, 1);
}

Matrix createMirrorMatrix(MirrorAxes axe) {
    if (axe == MirrorAxeX) return MatrixNew(1, 0, 0, -1);
    if (axe == MirrorAxeY) return MatrixNew(-1, 0, 0, 1);
    if (axe == MirrorAxeTL) return MatrixNew(0, 1, 1, 0);
    if (axe == MirrorAxeTR) return MatrixNew(0, -1, -1, 0);
    return createIdentityMatrix();
}

Matrix multiplyMatrix(Matrix left, Matrix right) {
    return MatrixNew(
        left.A * right.A + left.B * right.C,  left.A * right.B + left.B * right.D,
        left.C * right.A + left.D * right.C,  left.C * right.B + left.D * right.D
    );
}

Matrix invertMatrix(Matrix matrix) {
    int32 det = determinantMatrix(matrix);
    if (det == 0) return createIdentityMatrix();
    return MatrixNew(matrix.D / det, -matrix.B / det, -matrix.C / det, matrix.A / det);
}

Matrix transposeMatrix(Matrix matrix) {
    return MatrixNew(matrix.A, matrix.C, matrix.B, matrix.D);
}


Matrix3 createIdentityMatrix3() {
    return Matrix3New(1, 0, 0, 1, 0, 0);
}

Matrix3 createTranslationMatrix3(Vector translation) {
    return Matrix3New(1, 0, 0, 1, translation.X, translation.Y);
}

Matrix3 createRotationMatrix3(int32 degrees) {
    double rad = DEG_TO_RAD(degrees);
    return Matrix3New((int32)cos(rad), (int32)-sin(rad), (int32)sin(rad), (int32)cos(rad), 0, 0);
}

Matrix3 createScaleMatrix3(int32 scaleX, int32 scaleY) {
    return Matrix3New(scaleX, 0, 0, scaleY, 0, 0);
}

Matrix3 createShearMatrix3(int32 shearX, int32 shearY) {
    return Matrix3New(1, shearX, shearY, 1, 0, 0);
}

Matrix3 createMirrorMatrix3(MirrorAxes axe) {
    Matrix m = createMirrorMatrix(axe);
    return Matrix3New(m.A, m.B, m.C, m.D, 0, 0);
}

Matrix3 multiplyMatrix3(Matrix3 left, Matrix3 right) {
    return Matrix3New(
        left.A * right.A + left.B * right.C,
        left.A * right.B + left.B * right.E,
        left.C * right.A + left.E * right.C,
        left.C * right.B + left.E * right.E,
        left.D * right.A + left.F * right.C + right.D,
        left.D * right.B + left.F * right.E + right.F
    );
}

Matrix3 invertMatrix3(Matrix3 matrix) {
    int32 det = determinantMatrix3(matrix);
    if (det == 0) return createIdentityMatrix3();
    
    int32 invA = matrix.E / det;
    int32 invB = -matrix.B / det;
    int32 invC = -matrix.C / det;
    int32 invE = matrix.A / det;
    int32 invD = (matrix.C * matrix.F - matrix.D * matrix.E) / det;
    int32 invF = (matrix.B * matrix.D - matrix.A * matrix.F) / det;
    
    return Matrix3New(invA, invB, invC, invE, invD, invF);
}

Matrix3 transposeMatrix3(Matrix3 matrix) {
    return Matrix3New(matrix.A, matrix.C, matrix.B, matrix.E, matrix.D, matrix.F);
}