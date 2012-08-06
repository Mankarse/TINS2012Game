#ifndef TINSGame2012_Bullet_h
#define TINSGame2012_Bullet_h

class Enemy;

class Bullet : Renderable {
    void update(Dragon& player);
    void update(std::vector<Enemy>& enemies);
};

#endif
