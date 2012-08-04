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
class GroundHeightmap {
    private:
    int resolution;
    int width;
    std::vector<double> data;
    
    public: 
    GroundHeightmap(int newRes, int newWidth, std::vector<double> const& newData);
    
    double getInterpolatedPoint(double xPos);
    
    
};

#endif /* defined(__TINSGame2012__GroundHeightmap__) */
