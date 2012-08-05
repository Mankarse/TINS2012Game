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
    double p1;
    double p2;
    double proportion;
    p1 = data[(int)floor(xPos) % data.size()];
    p2 = data[(int)ceil(xPos) % data.size()];
    proportion = xPos - floor(xPos);
    return p1 + ((p2 - p1) * proportion);
}

double GroundHeightmap::getInterpolatedWorldPoint(double xPos) const{
    return getInterpolatedPoint(xPos / resolution);
}