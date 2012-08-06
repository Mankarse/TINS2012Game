#ifndef TINSGame2012_Bullet_h
#define TINSGame2012_Bullet_h

#include "Dragon.h"
#include "clone_ptr.h"
class Dragon;
class Enemy;
class BulletImplementation {
public:
    virtual void update(Dragon& player)=0;
    virtual void update(std::vector<Enemy>& enemies)=0;
    virtual BulletImplementation* clone() const=0;
    virtual ~BulletImplementation(){}
    virtual void pickRenderQueue(RenderQueueSet& queues) const = 0;
    virtual bool shouldDie() const = 0;
};

class Bullet {
public:
    explicit Bullet(BulletImplementation* impl): impl(impl){}
    void update(std::vector<Enemy>& enemies) {
        return impl->update(enemies);
    }
    void update(Dragon& player) {
        return impl->update(player);
    }
    void pickRenderQueue(RenderQueueSet& queues) const{
        impl->pickRenderQueue(queues);
    }
    bool shouldDie() const {
        return impl->shouldDie();
    }
private:
    clone_ptr<BulletImplementation> impl;
};

#endif
