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
        mode = Flying;
        foreLeg = Point2D(20, 15);
        hindLeg = Point2D(-20, 15);
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
                
                velocity *= 0.95;
                
                worldPosition += velocity / 60.;
                
                // Use al_transforms to modify the leg positions with rotation (later)
                Point2D actualForelegPoint(worldPosition + foreLeg);
                Point2D actualHindlegPoint(worldPosition + hindLeg);
                if(ground->getInterpolatedWorldPoint(actualForelegPoint.x) < actualForelegPoint.y ||
                    ground->getInterpolatedWorldPoint(actualHindlegPoint.x) < actualHindlegPoint.y) {
                    // This should rotate the dragon until both feet are on the ground, and only then change mode
                    mode = Grounded;
                    /*
                    worldPosition.y =
                    ground->getInterpolatedPoint(actualHindlegPoint.x) < ground->getInterpolatedPoint(actualForelegPoint.x) ?
                    ground->getInterpolatedPoint(actualForelegPoint.x) + foreLeg.y : ground->getInterpolatedPoint(actualHindlegPoint.x) + hindLeg.y;
                    */
                    velocity.y = 0;
                }
                
                break;
            }
            case Grounded:
            {
                velocity.x *= 0.9;
                if(al_key_down(&input.keyboardState, ALLEGRO_KEY_D)) {
                    velocity.x += 20;
                }
                if(al_key_down(&input.keyboardState, ALLEGRO_KEY_A)) {
                    velocity.x -= 20;
                }
                
                worldPosition.x += (velocity.x / 60.);
                worldPosition.y = ground->getInterpolatedWorldPoint(worldPosition.x);
                break;
            }
        }
    }
    void Dragon::renderStep(Point2D screenPos) const {
        Point2D drawPos = worldPosition - screenPos;
        al_draw_filled_rectangle(drawPos.x - 20, drawPos.y - 15, drawPos.x + 20, drawPos.y + 15, al_map_rgb(50, 160, 170));
    }