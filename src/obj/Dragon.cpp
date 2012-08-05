//
//  Dragon.cpp
//  TINSGame2012
//
//  Created by Evan Wallace on 4/08/12.
//
//

#include "Dragon.h"
#include <iostream>

    static const double airResistance = 0.997;
    static const double landResistance = 0.9;
    static const double flightCost = 15.;
    static const double staminaRegenTime = 5.;

    void Dragon::init() {
        worldPosition = Point2D(0, 0);
        mode = Flying;
        foreLeg = Point2D(7, 15);
        hindLeg = Point2D(-7, 15);
        stats.size = 10;
        stats.wingspan = 3;
        stats.stamina = 60;
        currentStamina = stats.stamina;
    }
    
    Dragon::Dragon() :
    worldPosition(0, 0),
    mode(Flying),
    foreLeg(7, 15),
    hindLeg(-7, 15)
    {
        stats.size = 10;
        stats.wingspan = 3;
        stats.stamina = 60;
        currentStamina = stats.stamina;
    }
    
    Dragon::Dragon(DragonStats newStats) :
    worldPosition(0, 0),
    mode(Flying),
    foreLeg(7, 15),
    hindLeg(-7, 15),
    stats(newStats)
    {
        currentStamina = stats.stamina;
    }
    
    void Dragon::assignHeightmap(GroundHeightmap *newGround) {
        ground = newGround;
    }
    
    ScreenFlipMode Dragon::physicsStep(InputState const& input) {
    
        bool shouldJump(false);
        for (std::vector<ALLEGRO_EVENT>::const_iterator it(input.events.begin()), end(input.events.end()); it != end; ++it)
        {
            ALLEGRO_EVENT const& event(*it);
            if(event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_W)
            {
                shouldJump = true;
            }
        }
        
        currentSkin = &baby;
        switch (mode) {
            case Flying:
            {
                velocity.y += (392.4 / 60.);
                
                if(shouldJump && currentStamina > flightCost)
                {
                    velocity.y -= 100 * stats.wingspan;
                    if(al_key_down(&input.keyboardState, ALLEGRO_KEY_D)) {
                        velocity.x += 50 * stats.wingspan;
                        velocity.y += 50 * stats.wingspan;
                    }
                    if(al_key_down(&input.keyboardState, ALLEGRO_KEY_A)) {
                        velocity.x -= 50 * stats.wingspan;
                        velocity.y += 50 * stats.wingspan;
                    }
                    currentStamina -= flightCost;
                }
                
                if(currentStamina <= 0 || al_key_down(&input.keyboardState, ALLEGRO_KEY_S)) {
                    // Rotate downwards, towards velocity
                } else if(velocity.y > 0) {
                    bool velocityRedirection = velocity.x > 0;
                    if(al_key_down(&input.keyboardState, ALLEGRO_KEY_D)) {
                        velocityRedirection = true;
                    }
                    if(al_key_down(&input.keyboardState, ALLEGRO_KEY_A)) {
                        velocityRedirection = false;
                    }
                    velocity.x = velocityRedirection ? velocity.x + (velocity.y * 0.1): velocity.x - (velocity.y * 0.1);
                    velocity.y *= 0.9;
                    currentStamina -= (10. / 60.);
                }
                velocity *= airResistance;
                
                worldPosition += velocity / 60.;
                
                // Use al_transforms to modify the leg positions with rotation (later)
                Point2D actualForelegPoint(worldPosition + foreLeg);
                Point2D actualHindlegPoint(worldPosition + hindLeg);
                if((ground->getInterpolatedWorldPoint(actualForelegPoint.x) < actualForelegPoint.y ||
                    ground->getInterpolatedWorldPoint(actualHindlegPoint.x) < actualHindlegPoint.y) && velocity.y > 0) {
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
                if(currentStamina < stats.stamina) {
                    currentStamina += ((stats.stamina * (1 / staminaRegenTime)) / 60);
                } else {
                    currentStamina = stats.stamina;
                }
                velocity.x *= landResistance;
                if(al_key_down(&input.keyboardState, ALLEGRO_KEY_D)) {
                    velocity.x += 20;
                }
                if(al_key_down(&input.keyboardState, ALLEGRO_KEY_A)) {
                    velocity.x -= 20;
                }
                if(shouldJump && currentStamina > flightCost)
                {
                    velocity.y -= 100 * stats.wingspan;
                    currentStamina -= flightCost;
                    mode = Flying;
                }
                worldPosition.x += (velocity.x / 60.);
                worldPosition.y = ground->getInterpolatedWorldPoint(worldPosition.x + foreLeg.x) - foreLeg.y;
                break;
            }
            
        }
        Point2D oldWorldPosition = worldPosition;
        worldPosition = ground->getLoopedCoordinate(worldPosition);
        if(oldWorldPosition != worldPosition) {
            return oldWorldPosition.x < worldPosition.x ? Right : Left;
            //std::cerr << "Screen flip! Time: " << al_get_time() << '\n';
        } else {
            return None;
        }
    }
    void Dragon::renderStep(Point2D screenPos) const {
        Point2D drawPos = worldPosition - screenPos;
        al_draw_filled_rectangle(drawPos.x - 20, drawPos.y - 15, drawPos.x + 20, drawPos.y + 15, al_map_rgb(50, 160, 170));
    }
    
