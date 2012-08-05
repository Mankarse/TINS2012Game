#ifndef TINSGame2012__Cave
#define TINSGame2012__Cave
#include "Scene.h"
#include "GameSave.h"

#include <allegro5/allegro.h>

class Cave: public Scene {
static bool isDisplayClosedEvent(ALLEGRO_EVENT const& e) {
    return e.type == ALLEGRO_EVENT_DISPLAY_CLOSE;
}
public:
    Cave(GameSave const& save): save(save) {}
    virtual Scene* update(InputState const& input) {
        if (std::find_if(
                input.events.begin(), input.events.end(), isDisplayClosedEvent)
                != input.events.end())
        {
            return 0;
        }
        return this;
    }
    virtual void renderTo(ALLEGRO_BITMAP* target) const {
        al_set_target_bitmap(target);
        al_clear_to_color(al_map_rgb(255, 0, 255));
    }
private:
    GameSave save;
};
#endif
