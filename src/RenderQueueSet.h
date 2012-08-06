#ifndef TINSGame2012_RenderQueueSet_h
#define TINSGame2012_RenderQueueSet_h
#include <vector>
class Renderable;

struct RenderQueueSet {
    std::vector<Renderable const*> farBackground;
    std::vector<Renderable const*> midBackground;
    std::vector<Renderable const*> nearBackground;
    std::vector<Renderable const*> middleGround;
    std::vector<Renderable const*> foreground;
};

#endif
