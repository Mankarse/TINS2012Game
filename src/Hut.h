#ifndef TINSGame2012_Hut_h
#define TINSGame2012_Hut_h
#include <vector>
#include "Villager.h"
class Hut : public SpawnerImplementation, private Renderable
{
    private:
    Point2D worldPosition;
    double lastSpawn;
    double timeToNextSpawn;
    public:
    virtual std::vector<Enemy> update(double playTime) {
        std::vector<Enemy> spawned;
        if(playTime >= lastSpawn + timeToNextSpawn){
            spawned.push_back(Enemy(new Villager(worldPosition.x)));
            lastSpawn = playTime;
            timeToNextSpawn = rand() % 20 + 10;
        }
        return spawned;
    }
    virtual SpawnerImplementation* clone() const {
        return new Hut(*this);
    }
    explicit Hut(Point2D const& position):
        worldPosition(position),
        lastSpawn(0),
        timeToNextSpawn(0)
    {}
    
    virtual void pickRenderQueue(RenderQueueSet& queues) const {}
    virtual ALLEGRO_BITMAP* getBitmap() const{
        return 0;
    }
    virtual Point2D getWorldPoint() const{
        return worldPosition;
    }
    virtual double getDepth() const{
        return 1;
    }
};

#endif
