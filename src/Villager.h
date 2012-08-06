#ifndef TINSGame2012_Villager_h
#define TINSGame2012_Villager_h
#include "Enemy.h"
#include "Renderable.h"
#include "Globals.h"
#include "SimpleAnimation.h"
#include <cstdlib>
class Villager : public EnemyImplementation, private Renderable {
    private:
    double currentPos;
    double currentVel;
    double viewDistance;
    double fearCooldown;
    double currentCooldown;
    bool wasStomped;
    SimpleAnimation anim;
    Point2D footPosition;
    GroundHeightmap const* heightmap;
    public:
    Villager(double startPos) :
        currentPos(startPos),
        currentVel(rand() % 100 > 50 ? -40 : 40),
        viewDistance(2000),
        fearCooldown(0),
        currentCooldown(0),
        heightmap(0),
        footPosition(0, -10),
        wasStomped(false),
        anim(5, "Villager")
    {}
    
    void beginTerror() {
        // Play sounds!
        switch(rand() % 4) {
            case (0):
            {
                al_play_sample(g_Samples["Screams1"], 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
                break;
            }
            case (1):
            {
                al_play_sample(g_Samples["Screams2"], 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
                break;
            }
            case (2):
            {
                al_play_sample(g_Samples["Screams3"], 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
                break;
            }
        }
    }
    
    virtual void update(std::vector<Bullet>& bulletlist, std::vector<Particle>& particleList, Dragon& player){
        currentPos += (currentVel / 60) /* (1 - dot(heightmap->getVector(currentPos), Point2D(0, -1)))*/;
        currentPos = heightmap->getLoopedOrdinate(currentPos);
        if(fearCooldown > 0) {
            fearCooldown -= 1./60.;
            if(fearCooldown <= 0)
            {
                fearCooldown = 0;
                currentVel = currentVel > 0 ? 40 : -40;
            }
        }
        if(distance(getWorldPoint(), player.worldPosition) < viewDistance)
        {
            if(!heightmap->linecast(getWorldPoint(), player.worldPosition)) {
                if(fearCooldown <= 0)
                {
                    
                    beginTerror();
                }
                fearCooldown = 10;
                int fearDirection = getWorldPoint().x < player.worldPosition.x ? -1 : 1;
                currentVel = fearDirection * 100;

            }
        }
        
        if(pointInRectInclusive(player.foreLegWorldPos().x, player.foreLegWorldPos().y, getBoundingBox())) {
            player.save.scoreDelta += 2;
            player.save.totalScore += 2;
            wasStomped = true;
        }
    }
    
    virtual void takeHit(double damage, Point2D vector) {
        wasStomped = true;
        
    }
    virtual Rect getBoundingBox() const {
        return Rect(getWorldPoint().x - 10, getWorldPoint().y - 15, 20, 30);
    }
    
    virtual bool hasHeightMap() const{
        return heightmap != 0;
    }
    virtual void assignHeightMap(GroundHeightmap const& newMap) {
        heightmap = &newMap;
    }
    
    virtual EnemyImplementation* clone() const{
        return new Villager(*this);
    }
    
    virtual void pickRenderQueue(RenderQueueSet& queues) const {
        queues.middleGround.push_back(this);
    }
    virtual ALLEGRO_BITMAP* getBitmap() const {
        return anim.getCurrentFrame();
    }
    virtual Point2D getWorldPoint() const {
        return Point2D(currentPos, heightmap->getInterpolatedWorldPoint(currentPos)) + footPosition;
    }
    virtual double getDepth() const {
        return 1;
    }
    virtual bool shouldDie() const {
        return wasStomped;
    }
};

#endif
