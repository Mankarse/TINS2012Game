#ifndef TINSGame2012_Scene_h
#define TINSGame2012_Scene_h
#include "InputState.h"
#include <allegro5/allegro.h>
class Scene {
public:
    virtual Scene* update(InputState const& input) {
        return 0;
    }
    virtual void renderTo(ALLEGRO_BITMAP* target) const {}
};
#endif
