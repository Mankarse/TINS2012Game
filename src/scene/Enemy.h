//
//  Enemy.h
//  TINSGame2012
//
//  Created by Evan Wallace on 5/08/12.
//
//

#ifndef TINSGame2012_Enemy_h
#define TINSGame2012_Enemy_h
#include <vector>
#include "GroundHeightMap.h"
#include "Renderable.h"
#include "Dragon.h"
#include "Bullet.h"
#include "Particle.h"
#include "clone_ptr.h"

class EnemyImplementation {
public:
    virtual void update(std::vector<Bullet>& bulletlist, std::vector<Particle>& particleList, Dragon& player)=0;
    virtual bool hasHeightMap() const=0;
    virtual void assignHeightMap(GroundHeightmap const& newMap)=0;
    virtual void pickRenderQueue(RenderQueueSet& queues) const = 0;
    virtual EnemyImplementation* clone() const=0;
    virtual ~EnemyImplementation(){}
};

class Enemy {
    public:
    Enemy(EnemyImplementation *impl):
        impl(impl)
    {
    }
    void update(std::vector<Bullet>& bulletlist, std::vector<Particle>& particleList, Dragon& player){
        impl->update(bulletlist, particleList, player);
    }
    bool hasHeightmap() {
        return impl->hasHeightMap();
    }
    void assignHeightmap(GroundHeightmap const& newMap) {
        impl->assignHeightMap(newMap);
    }
    void pickRenderQueue(RenderQueueSet& queues) const {
        impl->pickRenderQueue(queues);
    }
private:
    clone_ptr<EnemyImplementation> impl;
};

#endif
