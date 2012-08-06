#ifndef TINSGame2012_BackgroundObject_h
#define TINSGame2012_BackgroundObject_h

enum RenderLayer {
    FarBG,
    MidBG,
    NearBG,
    Middle,
    FG
};

class StaticObject : Renderable {
    private:
    ALLEGRO_BITMAP* image;
    Point2D worldPosition;
    double layer;
    RenderLayer ground;
    public:
    StaticObject(ALLEGRO_BITMAP* newImage, Point2D newPosition, double newLayer = 0.2, RenderLayer newGround = FarBG) :
    image(newImage),
    worldPosition(newPosition),
    layer(newLayer),
    ground(newGround){}
    
    
    virtual void pickRenderQueue(RenderQueueSet& queues) const {
        switch (ground) {
            case FarBG:
            {
                queues.farBackground.push_back(this);
                break;
            }
            case MidBG:
            {
                queues.midBackground.push_back(this);
                break;
            }
            case NearBG:
            {
                queues.nearBackground.push_back(this);
                break;
            }
            case Middle:
            {
                queues.middleGround.push_back(this);
                break;
            }
            case FG:
            {
                queues.foreground.push_back(this);
            }
        }
    }
    virtual ALLEGRO_BITMAP* getBitmap() const {
        return image;
    }
    virtual Point2D getWorldPoint() const {
        return worldPosition;
    }
    virtual double getDepth() const {
        return layer;
    }
};

#endif
