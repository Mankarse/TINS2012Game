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
#include <cmath>
#include "InputState.h"
#include "GroundHeightmap.h"
#include "Geometry.h"
#include "DragonStats.h"
#include "GameSave.h"
#include "Globals.h"
struct DragonSkin {
    ALLEGRO_BITMAP *body;
    ALLEGRO_BITMAP *neck;
    ALLEGRO_BITMAP *head;
};
enum DragonMode {
    Grounded,
    Flying,
};

enum ScreenFlipMode {
    Left,
    Right,
    None
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
    GameSave save;
    
    double currentStamina;
    double currentHealth;
    double currentCooldown;
    
    Point2D worldPosition;

    
    void assignHeightmap(GroundHeightmap* newGround);
    ScreenFlipMode physicsStep(InputState const& input);
    void renderStep(Point2D screenPos) const;
    Point2D foreLegWorldPos() const;
    Point2D hindLegWorldPos() const;
    
    Dragon();
    Dragon(GameSave const& newStats);
};




#endif /* defined(__TINSGame2012__Dragon__) */
