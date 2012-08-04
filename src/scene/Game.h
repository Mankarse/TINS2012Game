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
    double points[] = {1., 2., 3., 4.};
    
    return GroundHeightmap(1, static_cast<int>(length(points)), std::vector<double>(points, points + length(points)));
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
    Point2D screenCentre;
    GroundHeightmap ground;
    
    
    void init() {
        background = g_MenuBackground;
    }
    
    Point2D mouseToWorldPos(Point2D mousePosition) {
        int windowX;
        int windowY;
        al_get_window_position(al_get_current_display(), &windowX, &windowY);
        Point2D retV = mousePosition - Point2D(windowX, windowY);
        return retV - screenCentre;
    }
    
    virtual Scene* update(InputState const& input) {
        if (std::find_if(input.events.begin(), input.events.end(), isDisplayClosedEvent) != input.events.end()) {
            return 0;
        }
        // spawners!
        
        // Player!
        player.physicsStep(input);
        
        // Enemies!
        
        // My bullets!
        
        // Their bullets!
        
        // Particle effects update
        
        // Clean up dead things
        return this;
    }
    virtual void renderTo(ALLEGRO_BITMAP* target) const {
        // Render each queue, in order
        player.renderStep(screenCentre);
    }
};

#endif
