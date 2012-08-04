//
//  Geometry.h
//  TINSGame2012
//
//  Created by Evan Wallace on 5/08/12.
//
//

#ifndef TINSGame2012_Geometry_h
#define TINSGame2012_Geometry_h

struct Point2D {
    double x;
    double y;
    Point2D(double newX = 0, double newY = 0) : x(newX), y(newY) {}

};

inline Point2D& operator-=(Point2D &l, Point2D const& r) {
    l.x -= r.x;
    l.y -= r.y;
    return l;
}
inline Point2D operator-(Point2D l, Point2D const& r) {
    return l -= r;
}
inline Point2D& operator+=(Point2D &l, Point2D const& r) {
    l.x += r.x;
    l.y += r.y;
    return l;
}
inline Point2D operator+(Point2D l, Point2D const& r) {
    return l += r;
}
inline Point2D operator*=(Point2D &l, double const& r) {
    l.x *= r;
    l.y *= r;
    return l;
}
inline Point2D operator*=(Point2D &l, Point2D const& r) {
    l.x *= r.x;
    l.y *= r.y;
    return l;
}
inline Point2D operator*(Point2D l, double const& r) {
    return l *= r;
}
inline Point2D operator*(Point2D l, Point2D const& r) {
    return l *= r;
}

#endif
