//
//  GroundHeightmap.cpp
//  TINSGame2012
//
//  Created by Evan Wallace on 4/08/12.
//
//

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

    if(i1 < 0) {
        i1 += data.size();
    }
    if(i2 < 0) {
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

double GroundHeightmap::getTotalSize() const {
    return width * resolution;
}


