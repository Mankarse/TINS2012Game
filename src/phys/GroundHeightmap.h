//
//  GroundHeightmap.h
//  TINSGame2012
//
//  Created by Evan Wallace on 4/08/12.
//
//

#ifndef TINSGame2012__GroundHeightmap
#define TINSGame2012__GroundHeightmap
#include <vector>
#include <cmath>
#include <allegro5/allegro_primitives.h>
#include "Geometry.h"
class GroundHeightmap {
    private:
    int resolution;
    int width;
    std::vector<double> data;
    
    public: 
    GroundHeightmap(int newRes, int newWidth, std::vector<double> const& newData);
    void draw(Point2D screenPos) const;
    double getInterpolatedPoint(double xPos) const;
    double getInterpolatedWorldPoint(double xPos) const;
    Point2D getLoopedCoordinate(Point2D const& originalPoint) const;
    double getTotalSize() const;
    // A
    
    //double getGradient(double xPos) const;
};

#endif /* defined(__TINSGame2012__GroundHeightmap__) */
