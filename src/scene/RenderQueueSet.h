//
//  RenderQueueSet.h
//  TINSGame2012
//
//  Created by Evan Wallace on 5/08/12.
//
//

#ifndef TINSGame2012_RenderQueueSet_h
#define TINSGame2012_RenderQueueSet_h
#include <vector>
#include "Renderable.h"

struct RenderQueueSet {
    std::vector<Renderable*> farBackground;
    std::vector<Renderable*> nearBackground;
    std::vector<Renderable*> middleGround;
    std::vector<Renderable*> foreground;
};

#endif
