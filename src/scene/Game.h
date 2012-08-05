#ifndef TINSGame2012_Game_h
#define TINSGame2012_Game_h
#include "Scene.h"
#include "Dragon.h"
#include "GroundHeightmap.h"
#include "Geometry.h"
#include <allegro5/allegro.h>
#include "Globals.h"
#include <vector>



template<typename T, std::size_t N>
std::size_t length(T(&)[N]) {
    return N;
}

inline GroundHeightmap loadGlobalHeightmap() {
    double points[] = {-20., -30., -10., 15., 23., 17., 5., 0};
    
    return GroundHeightmap(100, static_cast<int>(length(points)), std::vector<double>(points, points + length(points)));
}

class Game:public Scene {
private:
    static bool isDisplayClosedEvent(ALLEGRO_EVENT const& e) {
        return e.type == ALLEGRO_EVENT_DISPLAY_CLOSE;
    }

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
        return mousePosition + screenCorner;
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
        player.physicsStep(input);
        
        
        
        screenCorner = ((mouseToWorldPos(mousePosition) + player.worldPosition) * 0.5) - Point2D(al_get_display_width(al_get_current_display()) * 0.5,
            al_get_display_height(al_get_current_display()) * 0.5);
        
        // Enemies!
        
        // My bullets!
        
        // Their bullets!
        
        // Particle effects update
        
        // Clean up dead things
        return this;
    }
    virtual void renderTo(ALLEGRO_BITMAP* target) const {
        // Collect renderables, add to queues
        
        // Render each queue, in order
        // Background:
        drawBitmapAtScreenPoint(background, Point2D(al_get_display_width(al_get_current_display()) * 0.5,
            al_get_display_height(al_get_current_display()) * 0.5));
        
        
        // Heightmap
        ground.draw(screenCorner);
        
        player.renderStep(screenCorner);
    }
};

#endif
