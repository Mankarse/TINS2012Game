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
    return (r.y - l.y) / (r.x - l.x);
}
inline double magnitude(Point2D const& obj) {
    return sqrt(pow(obj.x, 2) + pow(obj.y, 2));
}
inline Point2D normalised(Point2D const& obj) {
    return obj / magnitude(obj);
}
inline double angle(Point2D const& l, Point2D const& r) {
    return acos(dot(l, r) / (magnitude(l) * magnitude(r)));
}
inline Point2D lerp(Point2D const& l, Point2D const& r, double amount) {
    return l + ((r - l) * amount);
}
inline Point2D rotate(Point2D const& input, double amount) {
    return Point2D(input.x * cos(amount) - input.y * sin(amount), input.x * sin(amount) + input.y * cos(amount));
}
inline double lerp(double const& l, double const& r, double amount) {
    return l + ((r - l) * amount);
}

#endif
