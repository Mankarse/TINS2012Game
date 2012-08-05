//
//  Dragon.cpp
//  TINSGame2012
//
//  Created by Evan Wallace on 4/08/12.
//
//

#include "Dragon.h"

    void Dragon::init() {
        worldPosition = Point2D(20, -50);
        stats.size = 10;
        stats.wingspan = 5;
        stats.stamina = 20;
    }
    void Dragon::assignHeightmap(GroundHeightmap *newGround) {
        ground = newGround;
    }
    
    void Dragon::physicsStep(InputState const& input) {
        currentSkin = &baby;
        switch (mode) {
            case Flying:
            {
                velocity.y += (392.4 / 60.);
                
                worldPosition += velocity / 60.;
                
                break;
            }
            case Grounded:
            {
                velocity.x *= 0.9;
                
                break;
            }
        }
    }
    void Dragon::renderStep(Point2D screenPos) const {
        Point2D drawPos = worldPosition - screenPos;
        al_draw_filled_rectangle(drawPos.x - 20, drawPos.y - 15, drawPos.x + 20, drawPos.y + 15, al_map_rgb(50, 160, 170));
    }