#ifndef TINSGame2012_Villager_h
#define TINSGame2012_Villager_h
#include "Enemy.h"
#include "Renderable.h"
#include "Globals.h"
class Villager : public EnemyImplementation, private Renderable {
    private:
    double currentPos;
    double currentVel;
    double fearLevel;
    double viewDistance;
    double fearCooldown;
    double currentCooldown;
    GroundHeightmap const* heightmap;
    public:
    Villager(double startPos) :
        currentPos(startPos),
        currentVel(0),
        fearLevel(0),
        viewDistance(1000),
        fearCooldown(600),
        currentCooldown(0),
        heightmap(0)
    {}
    
    virtual void update(std::vector<Bullet>& bulletlist, std::vector<Particle>& particleList, Dragon& player){
        
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
        return g_NumberSheet10;
    }
    virtual Point2D getWorldPoint() const {
        return Point2D(currentPos, heightmap->getInterpolatedWorldPoint(currentPos));
    }
    virtual double getDepth() const {
        return 1;
    }
};

#endif
