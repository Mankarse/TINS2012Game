#include "GroundHeightmap.h"
#include <iostream>

GroundHeightmap::GroundHeightmap(int newRes, int newWidth, std::vector<double> const& newData):
    resolution(newRes),
    width(newWidth),
    data(newData) {
    }
    
void GroundHeightmap::draw(Point2D screenPos) const {
    int pos = 0;
    for (std::vector<double>::const_iterator it(data.begin()), end(data.end() - 1); it != end; ++it){
        double const& curPoint(*it);
        double const& nextPoint(*(it + 1));
        //int screenWidth = al_get_display_width(al_get_current_display());
        al_draw_line((pos * resolution) - screenPos.x, curPoint - screenPos.y, ((pos + 1) * resolution) - screenPos.x, nextPoint - screenPos.y, al_map_rgb(255, 0, 0), 3);
        al_draw_line(((pos * resolution) - screenPos.x) - (width * resolution), curPoint - screenPos.y, ((pos + 1) * resolution) - screenPos.x - (width * resolution), nextPoint - screenPos.y, al_map_rgb(255, 0, 0), 3);
        al_draw_line(((pos * resolution) - screenPos.x) + (width * resolution), curPoint - screenPos.y, ((pos + 1) * resolution) - screenPos.x + (width * resolution), nextPoint - screenPos.y, al_map_rgb(255, 0, 0), 3);
        
        pos++;
    }
    al_draw_line((pos * resolution) - screenPos.x, data[pos] - screenPos.y, ((pos + 1) * resolution) - screenPos.x, data[0] - screenPos.y, al_map_rgb(255, 0, 0), 3);
    al_draw_line(((pos * resolution) - screenPos.x) - (width * resolution), data[pos] - screenPos.y, ((pos + 1) * resolution) - screenPos.x - (width * resolution), data[0] - screenPos.y, al_map_rgb(255, 0, 0), 3);
    al_draw_line(((pos * resolution) - screenPos.x) + (width * resolution), data[pos] - screenPos.y, ((pos + 1) * resolution) - screenPos.x + (width * resolution), data[0] - screenPos.y, al_map_rgb(255, 0, 0), 3);
        
}
    
double GroundHeightmap::getInterpolatedPoint(double xPos) const{
    long i1(((long)floor(xPos)) % (long)data.size());
    long i2(((long)ceil(xPos)) % (long)data.size());

    while(i1 < 0) {
        i1 += data.size();
    }
    while(i2 < 0) {
        i2 += data.size();
    }
    //std::cout << "i1 " << i1 << '\n';
    //std::cout << "i2 " << i2 << '\n';
    double p1(data[i1]);
    double p2(data[i2]);
    //std::cerr << "p1: " << p1 << "\n";
    //std::cerr << "p2: " << p2 << "\n";
    double proportion(xPos - floor(xPos));
    return p1 + ((p2 - p1) * proportion);
}

double GroundHeightmap::getInterpolatedWorldPoint(double xPos) const{
    return getInterpolatedPoint(xPos / resolution);
}

Point2D GroundHeightmap::getLoopedCoordinate(Point2D const& originalPoint) const {
    double curPos = originalPoint.x;
    while (curPos > getTotalSize()) {
        curPos -= getTotalSize();
    }
    while (curPos < 0) {
        curPos += getTotalSize();
    }
    return Point2D(curPos, originalPoint.y);
}

double GroundHeightmap::getLoopedOrdinate(double xPos) const {
    while (xPos > getTotalSize()) {
        xPos -= getTotalSize();
    }
    while (xPos < 0) {
        xPos += getTotalSize();
    }
    return xPos;
}

double GroundHeightmap::getTotalSize() const {
    return width * resolution;
}

double GroundHeightmap::getGradient(double worldPoint) const {
    long i1(((long)floor(worldPoint)) % (long)data.size());
    long i2(((long)ceil(worldPoint)) % (long)data.size());

    while(i1 < 0) {
        i1 += data.size();
    }
    while(i2 < 0) {
        i2 += data.size();
    }
    //std::cout << "i1 " << i1 << '\n';
    //std::cout << "i2 " << i2 << '\n';
    double p1(data[i1]);
    double p2(data[i2]);
    return gradient(Point2D(i1, p1), Point2D(i2,p2));
}


Point2D GroundHeightmap::getVector(double worldPoint) const {
    long i1(((long)floor(worldPoint)) % (long)data.size());
    long i2(((long)ceil(worldPoint)) % (long)data.size());

    while(i1 < 0) {
        i1 += data.size();
    }
    while(i2 < 0) {
        i2 += data.size();
    }
    //std::cout << "i1 " << i1 << '\n';
    //std::cout << "i2 " << i2 << '\n';
    double p1(data[i1]);
    double p2(data[i2]);
    return Point2D(i2,p2) - Point2D(i1, p1);
}

bool GroundHeightmap::linecast(Point2D const& startPoint, Point2D const& endPoint) const {
    Point2D resolutionScalar(resolution, 1);
    Point2D leftPoint(startPoint.x < endPoint.x ? startPoint / resolutionScalar : endPoint / resolutionScalar);
    Point2D rightPoint(startPoint.x > endPoint.x ? startPoint / resolutionScalar : endPoint / resolutionScalar);
    for(long i = (long)floor(leftPoint.x); i <= (long)floor(rightPoint.x); ++i)
    {
        long currentIndex(i % (long)data.size());
        //std::cout << "Current index: " << currentIndex << '\n';
        long nextIndex((i + 1) % (long)data.size());
        //std::cout << "Next index: " << nextIndex << '\n';
        while(currentIndex < 0) {
            currentIndex += data.size();
        }
        while(nextIndex < 0) {
            nextIndex += data.size();
        }
        if((startPoint.y < endPoint.y ? startPoint.y : endPoint.y)
            >
            (data[currentIndex] > data[nextIndex] ? data[currentIndex] : data[nextIndex]))
        {
            continue;
        }
        Point2D curLineStart(i, data[currentIndex]);
        Point2D curLineEnd(i + 1, data[nextIndex]);
        double denominator(
            ((rightPoint.y - leftPoint.y) * (curLineEnd.x - curLineStart.x)) -
            ((rightPoint.x - leftPoint.x) * (curLineEnd.y - curLineStart.y)));
        if(denominator == 0) {
            continue;
        }
        double numeA(
            ((rightPoint.x - leftPoint.x) * (curLineStart.y - leftPoint.y)) -
            ((rightPoint.y - leftPoint.y) * (curLineStart.x - leftPoint.x))
        );
        double numeB(
            ((curLineEnd.x - curLineStart.x) * (curLineStart.y - leftPoint.y)) -
            ((curLineEnd.y - curLineStart.y) * (curLineStart.x - leftPoint.x))
        );
        double ua = numeA / denominator;
        double ub = numeB / denominator;
        
        if(ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1)
        {
            /*
            al_draw_filled_circle(((curLineStart.x + ua * (curLineEnd.x - curLineStart.x)) * resolution) - screenCorner.x,
                (curLineStart.y + ua * (curLineEnd.y - curLineStart.y)) - screenCorner.y, 4, al_map_rgb(0, 0, 255));
             */   
            //intersection = Point2D((curLineStart.x + ua * (curLineEnd.x - curLineStart.x) + i) * resolution,
            //curLineStart.y + ua * (curLineEnd.y - curLineStart.y));
            return true;
        }
        
    }
    return false;
}


