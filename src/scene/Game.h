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
#include "RenderQueueSet.h"
#include "Renderable.h"
#include "StaticObject.h"
#include "Bullet.h"
#include "Particle.h"
#include "Enemy.h"

template<typename T, std::size_t N>
std::size_t length(T(&)[N]) {
    return N;
}

inline GroundHeightmap loadGlobalHeightmap() {
    double points[] = {120., 110., 70., 79., 95., 190., 270., 270., 270., 270., 270., 270, 270., 270., 270, 260., 240.,
    100, 95, 90, 100, 190, 280, 285, 283, 280, 260, 230, 180, 110, 70, 75, 120, 170, 200, 140, 110, 110, 95, 105};
    
    return GroundHeightmap(100, static_cast<int>(length(points)), std::vector<double>(points, points + length(points)));
}

class Game:public Scene {
private:
    static bool isDisplayClosedEvent(ALLEGRO_EVENT const& e) {
        return e.type == ALLEGRO_EVENT_DISPLAY_CLOSE;
    }
    
    std::vector<StaticObject> staticRenderables;
    std::vector<Enemy> enemies;
    std::vector<Particle> particles;
    std::vector<Bullet> bullets;

public:
    Game() :
    ground(loadGlobalHeightmap()){}

    Dragon player;
    Point2D screenCorner;
    Point2D mousePosition;
    GroundHeightmap ground;
    
    
    void init() {
        player.init();
        player.assignHeightmap(&ground);
        al_init_primitives_addon();
        screenCorner = Point2D(0, al_get_display_height(al_get_current_display()) * -0.8);
    }
    
    Point2D mouseToWorldPos(Point2D const& mousePosition) const {
        Point2D retV(mousePosition + screenCorner);
        return retV;
    }
    
    Point2D worldToScreenPoint(Point2D const& worldPosition, double layer = 1) const {
        return (worldPosition - screenCorner) * layer;
    }
    
    void drawBitmapAtWorldPoint(ALLEGRO_BITMAP* image, Point2D point, double layer = 1) const {
        Point2D screenPos = worldToScreenPoint(point, layer);
        al_draw_scaled_bitmap(
        image,
        0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
        screenPos.x - ((al_get_bitmap_width(image)*0.5) * layer), screenPos.y - ((al_get_bitmap_height(image)*0.5) * layer),
        al_get_bitmap_width(image) * layer, al_get_bitmap_height(image) * layer,
        0);
    }
    void drawBitmapAtScreenPoint(ALLEGRO_BITMAP* image, Point2D point) const {
        al_draw_scaled_bitmap(
        image,
        0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
        point.x - ((al_get_bitmap_width(image)*0.5)), point.y - ((al_get_bitmap_height(image)*0.5)),
        al_get_bitmap_width(image), al_get_bitmap_height(image),
        0);
    }
    void drawBackground(ALLEGRO_BITMAP* image, double depth) const {
        double layer = 0.6;
        double basePosition = ground.getTotalSize() / 2;
        double positionOffset = al_get_time() * 15; // Look at the clouds move! Happy now?
        Point2D screenspaceCentre (worldToScreenPoint(Point2D(basePosition + positionOffset, 300), layer));
        //std::cout << screenCorner.x << '\n';
        if(screenCorner.y - positionOffset < screenspaceCentre.y - (al_get_bitmap_height(image) * 0.5)) {
            screenspaceCentre.y -= al_get_bitmap_height(image);
            al_draw_scaled_bitmap(
            image,
            0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
            screenspaceCentre.x - ((al_get_bitmap_width(image)*0.5)), screenspaceCentre.y - ((al_get_bitmap_height(image)*0.5)),
            al_get_bitmap_width(image)*4, al_get_bitmap_height(image),
            0);
            screenspaceCentre.y += al_get_bitmap_height(image);
        }
        al_draw_scaled_bitmap(
        image,
        0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
        screenspaceCentre.x - ((al_get_bitmap_width(image)*0.5)), screenspaceCentre.y - ((al_get_bitmap_height(image)*0.5)),
        al_get_bitmap_width(image), al_get_bitmap_height(image),
        0);
        if(screenCorner.x - positionOffset < 0)
        {
            screenspaceCentre = worldToScreenPoint(Point2D((-basePosition) + positionOffset, 300), layer);
            if(screenCorner.y < screenspaceCentre.y - (al_get_bitmap_height(image) * 0.5)) {
                screenspaceCentre.y -= al_get_bitmap_height(image);
                al_draw_scaled_bitmap(
                image,
                0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
                screenspaceCentre.x - ((al_get_bitmap_width(image)*0.5)), screenspaceCentre.y - ((al_get_bitmap_height(image)*0.5)),
                al_get_bitmap_width(image)*4, al_get_bitmap_height(image),
                0);
                screenspaceCentre.y += al_get_bitmap_height(image);
            }
            al_draw_scaled_bitmap(
            image,
            0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
            screenspaceCentre.x - ((al_get_bitmap_width(image)*0.5)), screenspaceCentre.y - ((al_get_bitmap_height(image)*0.5)),
            al_get_bitmap_width(image), al_get_bitmap_height(image),
            0);
        }
        if(screenCorner.x + al_get_display_width(al_get_current_display()) > ground.getTotalSize() / 2)
        {
            screenspaceCentre = worldToScreenPoint(Point2D((basePosition + ground.getTotalSize() + positionOffset), 300), layer);
            if(screenCorner.y < screenspaceCentre.y - (al_get_bitmap_height(image) * 0.5)) {
                screenspaceCentre.y -= al_get_bitmap_height(image);
                al_draw_scaled_bitmap(
                image,
                0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
                screenspaceCentre.x - ((al_get_bitmap_width(image)*0.5)), screenspaceCentre.y - ((al_get_bitmap_height(image)*0.5)),
                al_get_bitmap_width(image)*4, al_get_bitmap_height(image),
                0);
                screenspaceCentre.y += al_get_bitmap_height(image);
            }
            al_draw_scaled_bitmap(
            image,
            0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
            screenspaceCentre.x - ((al_get_bitmap_width(image)*0.5)), screenspaceCentre.y - ((al_get_bitmap_height(image)*0.5)),
            al_get_bitmap_width(image), al_get_bitmap_height(image),
            0);
        }
    }
    
    virtual Scene* update(InputState const& input) {
        if (std::find_if(input.events.begin(), input.events.end(), isDisplayClosedEvent) != input.events.end()) {
            return 0;
        }
        for (std::vector<ALLEGRO_EVENT>::const_iterator it(input.events.begin()), end(input.events.end()); it != end; ++it)
        {
            ALLEGRO_EVENT const& event(*it);
            if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
                mousePosition.x = event.mouse.x;
                mousePosition.y = event.mouse.y;
            }
        }
        
        // spawners!
        
        // Player!
        switch(player.physicsStep(input)) {
            case Left :
            {
                screenCorner.x -= ground.getTotalSize();
                break;
            }
            case Right :
            {
                screenCorner.x += ground.getTotalSize();
                break;
            }
            case None :
            {
                break;
            }
            //screenCorner = ground.getLoopedCoordinate(screenCorner);
        }

        // Camera control
        screenCorner = ((mouseToWorldPos(mousePosition) + player.worldPosition) * 0.5) - Point2D(al_get_display_width(al_get_current_display()) * 0.5,
            al_get_display_height(al_get_current_display()) * 0.5);
        
        // Enemies!
        for(std::vector<Enemy>::iterator it(enemies.begin()), end(enemies.end()); it != end; ++it)
        {
            Enemy& curEnemy(*it);
            curEnemy.update(bullets, particles, player);
        }
        // My bullets!
        
        // Their bullets!
        
        // Particle effects update
        
        // Clean up dead things
        return this;
    }
    
    void renderQueue(std::vector<Renderable const*> const& queue) const {
        for (std::vector<Renderable const*>::const_iterator it(queue.begin()), end(queue.end()); it != end; ++it)
        {
            Renderable const& curObject(**it);
            Point2D position(curObject.getWorldPoint());
            // renders the object 3 times!
            drawBitmapAtWorldPoint(curObject.getBitmap(), position, curObject.getDepth());
            drawBitmapAtWorldPoint(curObject.getBitmap(), Point2D(position.x + ground.getTotalSize(), position.y), curObject.getDepth());
            drawBitmapAtWorldPoint(curObject.getBitmap(), Point2D(position.x - ground.getTotalSize(), position.y), curObject.getDepth());
        }
    }
    
    void preRender(RenderQueueSet* renderQueues) const {
        for (std::vector<StaticObject>::const_iterator it(staticRenderables.begin()), end(staticRenderables.end()); it != end; ++it)
        {
            StaticObject const& curObject(*it);
            curObject.pickRenderQueue(*renderQueues);
        }
    }
    
    void drawingPass(RenderQueueSet* renderQueues) const {
        // Render each queue, in order
        // Background:
        drawBackground(g_LevelSky, 0.5);
        
        renderQueue(renderQueues->farBackground);
        renderQueue(renderQueues->nearBackground);
        // Heightmap
        drawBitmapAtWorldPoint(g_LevelFG, Point2D(0,0));
        drawBitmapAtWorldPoint(g_LevelFG, Point2D(ground.getTotalSize(),0));
        drawBitmapAtWorldPoint(g_LevelFG, Point2D(-ground.getTotalSize(),0));
        Point2D worldTopCorner(-ground.getTotalSize(), 300);
        worldTopCorner = worldToScreenPoint(worldTopCorner);
        Point2D worldBottomCorner(ground.getTotalSize() * 2, 1000);
        worldBottomCorner = worldToScreenPoint(worldBottomCorner);
        al_draw_filled_rectangle(worldTopCorner.x, worldTopCorner.y, worldBottomCorner.x, worldBottomCorner.y, al_map_rgb(146, 120, 94));
        renderQueue(renderQueues->middleGround);
        player.renderStep(screenCorner);
        
        //ground.draw(screenCorner);
        renderQueue(renderQueues->foreground);
    }
    
    virtual void renderTo(ALLEGRO_BITMAP* target) const {
        // Collect renderables, add to queues
        // Background objects
        RenderQueueSet renderQueues;
        preRender(&renderQueues);
        
        // Now, render the collected queues
        drawingPass(&renderQueues);

    }
};

#endif
