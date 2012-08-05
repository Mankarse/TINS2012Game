#ifndef TINSGame2012__Cave
#define TINSGame2012__Cave
#include "Scene.h"
#include "GameSave.h"

class Cave: public Scene {
public:
    Cave(GameSave const& save) {
        
    }
    virtual Scene* update(InputState const& input);
    virtual void renderTo(ALLEGRO_BITMAP* target) const;
private:
    GameSave save;
};
#endif