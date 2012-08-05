#ifndef TINSGame2012_Game_h
#define TINSGame2012_Game_h
#include "Scene.h"
#include "Dragon.h"
#include "GroundHeightmap.h"
#include "Geometry.h"
#include <allegro5/allegro.h>
#include "Globals.h"
#include <vector>
#include <iostream>
#include <memory>
#include "RenderQueueSet.h"
#include "Renderable.h"
#include "StaticObject.h"
#include "Bullet.h"
#include "Particle.h"
#include "Enemy.h"
#include "GameSave.h"
#include "Rect.h"
#include "Cave.h"

class Game:public Scene {
public:
    Game();

    Game(GameSave savedGame);
    
    void init();
    virtual Scene* update(InputState const& input);    
    virtual void renderTo(ALLEGRO_BITMAP* target) const;    


private:
    Rect caveRect;
    std::vector<StaticObject> staticRenderables;
    std::vector<Enemy> enemies;
    std::vector<Particle> particles;
    std::vector<Bullet> bullets;
    
    Dragon player;
    Point2D screenCorner;
    Point2D mousePosition;
    GroundHeightmap ground;
    Point2D mouseToWorldPos(Point2D const& mousePosition) const;
    
    Point2D worldToScreenPoint(Point2D const& worldPosition, double layer = 1) const;
    void drawBitmapAtWorldPoint(ALLEGRO_BITMAP* image, Point2D point, double layer = 1) const;
    void drawBitmapAtScreenPoint(ALLEGRO_BITMAP* image, Point2D point) const;
    void drawBackground(ALLEGRO_BITMAP* image, double depth) const;
    

    
    void renderQueue(std::vector<Renderable const*> const& queue) const;
    void preRender(RenderQueueSet* renderQueues) const;
    void drawCave() const;
    void drawingPass(RenderQueueSet* renderQueues) const;
    
};

#endif
