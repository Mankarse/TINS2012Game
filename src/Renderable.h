#ifndef TINSGame2012_Renderable_h
#define TINSGame2012_Renderable_h
#include <allegro5/allegro.h>
#include "Geometry.h"
class RenderQueueSet;

class Renderable {
    public:
    virtual ALLEGRO_BITMAP* getBitmap() const = 0;
    virtual Point2D getWorldPoint() const = 0;
    virtual double getDepth() const = 0;
};

#endif
