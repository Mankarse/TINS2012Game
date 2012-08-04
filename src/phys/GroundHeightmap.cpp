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
    data(newData) {}
    
double GroundHeightmap::getInterpolatedPoint(double xPos) {
    double p1;
    double p2;
    double proportion;
    p1 = data[(int)floor(xPos)];
    p2 = data[(int)ceil(xPos)];
    proportion = xPos - floor(xPos);
    return p1 + ((p2 - p1) * proportion);
}