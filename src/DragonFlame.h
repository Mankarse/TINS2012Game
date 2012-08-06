//
//  DragonFlame.h
//  TINSGame2012
//
//  Created by Evan Wallace on 6/08/12.
//
//

#ifndef TINSGame2012_DragonFlame_h
#define TINSGame2012_DragonFlame_h
#include "Renderable.h"
#include "RenderQueueSet.h"
#include "Bullet.h"
#include "Rect.h"
#include "Enemy.h"
#include "Dragon.h"
#include "GroundHeightmap.h"
class Dragon;

class DragonFlame : public BulletImplementation, private Renderable
{
    private:
    Point2D worldPosition;
    Point2D velocity;
    double lifeTime;
    double startTime;
    SimpleAnimation anim;
    bool hitThing;
    GroundHeightmap* ground;
    public:
    virtual void update(Dragon& player) {} // Friendly bullet, not implemented
    
    virtual void update(std::vector<Enemy>& enemies) {
        worldPosition += velocity / 60.;
        velocity.y += (392.4 / 60.);
        velocity *= 0.99;
        for (std::vector<Enemy>::iterator it(enemies.begin()), end(enemies.end()); it != end; ++it) {
            Enemy curEnemy(*it);
            if(pointInRectInclusive(worldPosition.x, worldPosition.y, curEnemy.getBoundingBox())) {
                curEnemy.takeHit(0, Point2D());
                hitThing = true;
            }
        }
        if(ground->getInterpolatedWorldPoint(worldPosition.x) > worldPosition.y)
        {
            hitThing = true;
        }
        if(al_get_time() > lifeTime + startTime) {
            hitThing = true;
        }
    }
    virtual BulletImplementation* clone() const {
        return new DragonFlame(*this);
    }
    explicit DragonFlame(Point2D const& position, Point2D const& vel, GroundHeightmap* ground, double newLife):
        worldPosition(position),
        velocity(vel),
        ground(ground),
        hitThing(false),
        anim(7, "Flame"),
        lifeTime(newLife),
        startTime(al_get_time())
    {}
    
    virtual void pickRenderQueue(RenderQueueSet& queues) const {
        queues.foreground.push_back(this);
    }
    virtual ALLEGRO_BITMAP* getBitmap() const{
        return anim.getCurrentFrame();
    }
    virtual Point2D getWorldPoint() const{
        return worldPosition;
    }
    virtual double getDepth() const{
        return 1 + lerp(0, 0.2, (al_get_time() - startTime) / lifeTime);
    }
    virtual bool shouldDie() const {
        return hitThing;
    }
};

#endif
