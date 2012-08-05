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
    
    RenderQueueSet renderQueues;

    ALLEGRO_BITMAP* background;

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
        background = g_MenuBackground;
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
        screenPos.x - ((al_get_bitmap_width(image)*2) * layer), screenPos.y - ((al_get_bitmap_height(image)*2) * layer),
        al_get_bitmap_width(image)*4 * layer, al_get_bitmap_height(image)*4 * layer,
        0);
    }
    void drawBitmapAtScreenPoint(ALLEGRO_BITMAP* image, Point2D point) const {
        al_draw_scaled_bitmap(
        image,
        0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
        point.x - ((al_get_bitmap_width(image)*2)), point.y - ((al_get_bitmap_height(image)*2)),
        al_get_bitmap_width(image)*4, al_get_bitmap_height(image)*4,
        0);
    }
    void drawBackground(ALLEGRO_BITMAP* image, double depth) const {
        //double scalingFactor = 4;
        Point2D screenspaceCentre (worldToScreenPoint(Point2D(ground.getTotalSize() / 2,0), 0.4));
        //std::cout << screenspaceCentre.x << '\n';
        while (screenspaceCentre.y > al_get_display_height(al_get_current_display())) {
            screenspaceCentre.y -= 300;
        }
        while (screenspaceCentre.y < 0) {
            screenspaceCentre.y += 300;
        }
        al_draw_scaled_bitmap(
        image,
        0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
        screenspaceCentre.x - ((al_get_bitmap_width(image)*2)), screenspaceCentre.y - ((al_get_bitmap_height(image)*2)),
        al_get_bitmap_width(image)*4, al_get_bitmap_height(image)*4,
        0);
        al_draw_scaled_bitmap(
        image,
        0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
        screenspaceCentre.x - ((al_get_bitmap_width(image)*6)), screenspaceCentre.y - ((al_get_bitmap_height(image)*2)),
        al_get_bitmap_width(image)*4, al_get_bitmap_height(image)*4,
        0);
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
        
        // My bullets!
        
        // Their bullets!
        
        // Particle effects update
        
        // Clean up dead things
        return this;
    }
    
    void renderQueue(std::vector<Renderable*> const& queue) const {
        for (std::vector<Renderable*>::const_iterator it(queue.begin()), end(queue.end()); it != end; ++it)
        {
            Renderable const& curObject(**it);
            Point2D position(curObject.getWorldPoint());
            // renders the object 3 times!
            drawBitmapAtWorldPoint(curObject.getBitmap(), position, curObject.getDepth());
            drawBitmapAtWorldPoint(curObject.getBitmap(), Point2D(position.x + ground.getTotalSize(), position.y), curObject.getDepth());
            drawBitmapAtWorldPoint(curObject.getBitmap(), Point2D(position.x - ground.getTotalSize(), position.y), curObject.getDepth());
        }
    }
    
    virtual void renderTo(ALLEGRO_BITMAP* target) const {
        // Collect renderables, add to queues
        
        // Render each queue, in order
        // Background:
        drawBackground(g_LevelSky, 0.5);
        
        renderQueue(renderQueues.farBackground);
        renderQueue(renderQueues.nearBackground);
        // Heightmap
        drawBitmapAtWorldPoint(g_LevelFG, Point2D(0,0));
        drawBitmapAtWorldPoint(g_LevelFG, Point2D(ground.getTotalSize(),0));
        drawBitmapAtWorldPoint(g_LevelFG, Point2D(-ground.getTotalSize(),0));
        Point2D worldTopCorner(-ground.getTotalSize(), 300);
        worldTopCorner = worldToScreenPoint(worldTopCorner);
        Point2D worldBottomCorner(ground.getTotalSize() * 2, 1000);
        worldBottomCorner = worldToScreenPoint(worldBottomCorner);
        al_draw_filled_rectangle(worldTopCorner.x, worldTopCorner.y, worldBottomCorner.x, worldBottomCorner.y, al_map_rgb(146, 120, 94));
        renderQueue(renderQueues.middleGround);
        player.renderStep(screenCorner);
        
        //ground.draw(screenCorner);
        renderQueue(renderQueues.foreground);
    }
};

#endif
