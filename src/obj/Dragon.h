//
//  Dragon.h
//  TINSGame2012
//
//  Created by Evan Wallace on 4/08/12.
//
//

#ifndef TINSGame2012__Dragon
#define TINSGame2012__Dragon
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "InputState.h"
#include "GroundHeightmap.h"
#include "Geometry.h"
#include "DragonStats.h"
struct DragonSkin {
    ALLEGRO_BITMAP *body;
    ALLEGRO_BITMAP *neck;
    ALLEGRO_BITMAP *head;
};
enum DragonMode {
    Grounded,
    Flying,
};

class Dragon {
    private:
    DragonSkin baby;
    DragonSkin adult;
    DragonSkin ancient;
    
    DragonSkin* currentSkin;
    
    GroundHeightmap* ground;
    
    // current physics state variables
    double currentRotation;
    
    Point2D localNeckOffset;
    double currentNeckRotation;
    Point2D localHeadOffest;
    double currentHeadRotation;
    
    Point2D velocity;
    DragonMode mode;
    
    Point2D foreLeg;
    Point2D hindLeg;
    
    public:
    DragonStats stats;
    Point2D worldPosition;
    void init();
    void assignHeightmap(GroundHeightmap* newGround);
    void physicsStep(InputState const& input);
    void renderStep(Point2D screenPos) const;
};




#endif /* defined(__TINSGame2012__Dragon__) */
