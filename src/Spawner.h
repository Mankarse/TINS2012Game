#ifndef TINSGame2012_Spawner_h
#define TINSGame2012_Spawner_h
#include <vector>
#include "Enemy.h"
#include "Geometry.h"
#include "clone_ptr.h"

class SpawnerImplementation {
public:
    virtual std::vector<Enemy> update(double playTime)=0;
    virtual SpawnerImplementation* clone() const=0;
    virtual ~SpawnerImplementation(){}
    virtual void pickRenderQueue(RenderQueueSet& queues) const = 0;
};

class Spawner {
public:
    explicit Spawner(SpawnerImplementation* impl): impl(impl){}
    std::vector<Enemy> update(double playTime) {
        return impl->update(playTime);
    }
    void pickRenderQueue(RenderQueueSet& queues) const{
        impl->pickRenderQueue(queues);
    }
private:
    clone_ptr<SpawnerImplementation> impl;
};

#endif
