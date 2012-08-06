#ifndef TINSGame2012_Rect_h
#define TINSGame2012_Rect_h
struct Rect {
    Rect(double x, double y, double width, double height) :
        x(x), y(y), width(width), height(height) {}
    double x;
    double y;
    double width;
    double height;
    
    double maxX() const {
        return x + width;
    }
    double maxY() const {
        return y + height;
    }
};

inline Rect operator*(Rect const& l, double r) {
    return Rect(l.x*r, l.y*r, l.width*r, l.height*r);
}

inline bool pointInRectInclusive(double x, double y, Rect const& rect) {
    return x >= rect.x && x <= rect.x+rect.width && y >= rect.y && y <= rect.y + rect.height;
}

#endif
