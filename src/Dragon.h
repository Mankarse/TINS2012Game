#ifndef TINSGame2012__Dragon
#define TINSGame2012__Dragon
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include "InputState.h"
#include "GroundHeightmap.h"
#include "Geometry.h"
#include "Globals.h"
#include "DragonStats.h"
#include "GameSave.h"
#include "SimpleAnimation.h"
#include "DragonFlame.h"
#include <vector>
struct DragonSkin {
    SimpleAnimation run;
    SimpleAnimation dive;
    SimpleAnimation glide;
    SimpleAnimation flap;
    SimpleAnimation jump;
    ALLEGRO_BITMAP* stand;
    
    DragonSkin() :
    run(10, "dragonsmallrun"),
    dive(10, "DragonSmallDive"),
    glide(10, "DragonSmallGlide"),
    flap(10, "DragonSmallFly"),
    jump(15, "DragonSmallJump"),
    stand(g_Bitmaps["DragonSmallStand"])
    {}
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
    
    DragonSkin* currentSkin;
    
    GroundHeightmap* ground;
    
    // current physics state variables
    double currentRotation;

    ScreenFlipMode currentDirection;
    
    
    Point2D velocity;
    DragonMode mode;
    Point2D foreLeg;
    Point2D hindLeg;
    
    double jumpTime;
    bool isBreathing;
    
    void renderRotated(ALLEGRO_BITMAP* image, Point2D screenPos, int flags = 0) const;
    void fireBreath(Point2D mainVelocity, double lifeTime, std::vector<Bullet> bulletList) const;
    public:
    GameSave save;
    
    double currentStamina;
    double currentHealth;
    double currentCooldown;
    
    Point2D worldPosition;
    
    void assignHeightmap(GroundHeightmap* newGround);
    ScreenFlipMode physicsStep(InputState const& input, Point2D const& screenCorner, std::vector<Bullet> bulletList);
    void renderStep(Point2D screenPos) const;
    Point2D foreLegWorldPos() const;
    Point2D hindLegWorldPos() const;
    
    
    Dragon();
    Dragon(GameSave const& newStats);
};

#endif /* defined(TINSGame2012__Dragon) */
