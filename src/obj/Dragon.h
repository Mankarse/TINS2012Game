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
#include "InputState.h"
#include "GroundHeightmap.h"
#include "Geometry.h"
struct DragonSkin {
    ALLEGRO_BITMAP *body;
    ALLEGRO_BITMAP *neck;
    ALLEGRO_BITMAP *head;
};

class Dragon {
    private:
    DragonSkin baby;
    DragonSkin adult;
    DragonSkin ancient;
    
    DragonSkin* currentSkin;
    
    GroundHeightmap* ground;
    
    // current physics state variables
    Point2D worldPosition;
    Point2D localNeckOffset;
    double currentNeckRotation;
    Point2D localHeadOffest;
    double currentHeadRotation;
    
    public:
    void physicsStep(InputState const& input);
    void renderStep(Point2D screenPos) const;
};




#endif /* defined(__TINSGame2012__Dragon__) */
