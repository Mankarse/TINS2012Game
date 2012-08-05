//
//  GroundHeightmap.cpp
//  TINSGame2012
//
//  Created by Evan Wallace on 4/08/12.
//
//

#include "GroundHeightmap.h"

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
        al_draw_line((pos++ * resolution) - screenPos.x, curPoint - screenPos.y, (pos * resolution) - screenPos.x, nextPoint - screenPos.y, al_map_rgb(255, 0, 0), 3);
    }
}
    
double GroundHeightmap::getInterpolatedPoint(double xPos) const{
    double p1;
    double p2;
    double proportion;
    p1 = data[(int)floor(xPos)];
    p2 = data[(int)ceil(xPos)];
    proportion = xPos - floor(xPos);
    return p1 + ((p2 - p1) * proportion);
}