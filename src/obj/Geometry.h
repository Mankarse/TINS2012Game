//
//  Geometry.h
//  TINSGame2012
//
//  Created by Evan Wallace on 5/08/12.
//
//

#ifndef TINSGame2012_Geometry_h
#define TINSGame2012_Geometry_h
#include <cmath>
struct Point2D {
    double x;
    double y;
    Point2D(double newX, double newY) : x(newX), y(newY) {}
    Point2D(): x(), y(){}
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

inline Point2D operator/=(Point2D &l, double const& r) {
    l.x /= r;
    l.y /= r;
    return l;
}
inline Point2D operator/=(Point2D &l, Point2D const& r) {
    l.x /= r.x;
    l.y /= r.y;
    return l;
}
inline Point2D operator/(Point2D l, double const& r) {
    return l /= r;
}
inline Point2D operator/(Point2D l, Point2D const& r) {
    return l /= r;
}

inline bool operator==(Point2D const& l, Point2D const& r) {
    return(l.x == r.x && l.y == r.y);
}
inline bool operator!=(Point2D const& l, Point2D const& r) {
    return(l.x != r.x || l.y != r.y);
}
inline double dot(Point2D const& l, Point2D const& r) {
    return l.x * r.x + l.y * r.y;
}
inline double distance(Point2D const& l, Point2D const& r) {
    return sqrt(pow((r - l).x, 2) * pow((r - l).y, 2));
}
inline double gradient(Point2D const& l, Point2D const& r) {
    return (r.y - l.y) / (r.x - l.x)
}

#endif
