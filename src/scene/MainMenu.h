#ifndef TINSGame2012_MainMenu_h
#define TINSGame2012_MainMenu_h
#include "Scene.h"
class MainMenu:public Scene {
public:
    void init() {
        
    }
    virtual Scene* update(InputState const& input) {
        return this;
    }
    virtual void renderTo(ALLEGRO_BITMAP* target) const {
        
    }
};
#endif
