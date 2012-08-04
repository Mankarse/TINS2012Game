#ifndef TINSGame2012_MainMenu_h
#define TINSGame2012_MainMenu_h
#include <iostream>
#include "Scene.h"

inline bool isDisplayClosedEvent(ALLEGRO_EVENT const& e) {
    return e.type == ALLEGRO_EVENT_DISPLAY_CLOSE;
}

class MainMenu:public Scene {
public:
    void init() {
    }
    virtual Scene* update(InputState const& input) {
        if (std::find_if(input.events.begin(), input.events.end(), isDisplayClosedEvent) != input.events.end()) {
            return 0;
        }
        return this;
    }
    virtual void renderTo(ALLEGRO_BITMAP* target) const {
        
    }
};
#endif
