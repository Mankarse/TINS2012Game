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
class Enemy : Renderable{
    private:
    GroundHeightMap* heightmap;
    
    public:
    virtual void update(std::vector<Bullet>& bulletlist, std::vector<Particle>& particleList, Dragon& player) = 0;
    void assignHeightmap(GroundHeightMap* newHeights) {
        heightmap = newHeights;
    }
};

#endif
