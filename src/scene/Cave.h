#ifndef TINSGame2012__Cave
#define TINSGame2012__Cave
#include "Scene.h"
#include "GameSave.h"

#include <allegro5/allegro.h>
#include <cmath>
#include <algorithm>
#include "Rect.h"
#include "Primes.h"
#include "Game.h"

inline DragonStats getUpgradeAmounts(unsigned prime, GameSave const& save) {
    DragonStats stats;
    stats.size = 0.1 * std::pow(1.1, static_cast<double>(prime));
    stats.wingspan = 0.1 * std::pow(1.1, static_cast<double>(prime));
    stats.stamina = 0.1 * std::pow(1.1, static_cast<double>(prime));
    stats.fireStrength = 0.1 * std::pow(1.1, static_cast<double>(prime));
    stats.fireCooldown = 0.1 * std::pow(1.1, static_cast<double>(prime));
    return stats;
}

//TODO
static int const primeTileHeight = 10;

class Cave: public Scene {
static bool isDisplayClosedEvent(ALLEGRO_EVENT const& e) {
    return e.type == ALLEGRO_EVENT_DISPLAY_CLOSE;
}
    void nextTile() {
        stackClearance += primeTileHeight;
        factors.pop_back();
        if (!factors.empty()) {
            currentDeltas = getUpgradeAmounts(factors.back(), save);
        }
    }
public:
    Cave(GameSave const& save):
        stackClearance(0),
        save(save),
        factors(factorize(save.scoreDelta))
    {
        this->save.scoreDelta = 0;
    }
    virtual Scene* update(InputState const& input) {
        if (std::find_if(
                input.events.begin(), input.events.end(), isDisplayClosedEvent)
                != input.events.end())
        {
            return 0;
        }
        
        for (std::vector<ALLEGRO_EVENT>::const_iterator it(input.events.begin()), end(input.events.end()); it != end; ++it)
        {
            ALLEGRO_EVENT const& event(*it);
            if (selectedButton != -1) {
                switch (event.type) {
                case ALLEGRO_EVENT_MOUSE_AXES: {
                    pressedButton = pointInRectInclusive(event.mouse.x, event.mouse.y, buttonPositions[selectedButton]) ? selectedButton : -1;
                    break;
                }
                case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
                    if(event.mouse.button == 1) {
                        if (pointInRectInclusive(event.mouse.x, event.mouse.y, buttonPositions[selectedButton])) {
                            switch (selectedButton) {
                                case 0: {
                                    //TODO games loading saves.
                                    std::auto_ptr<Game> game(new Game(/*save*/));
                                    game->init();
                                    return game.release();
                                }
                                case 1: save.stats.size += currentDeltas.size; nextTile(); break;
                                case 2: save.stats.wingspan += currentDeltas.wingspan; nextTile(); break;
                                case 3: save.stats.stamina += currentDeltas.stamina; nextTile(); break;
                                case 4: save.stats.fireStrength += currentDeltas.fireStrength; nextTile(); break;
                                case 5: save.stats.fireCooldown += currentDeltas.fireCooldown; nextTile(); break;
                            }
                        }
                        pressedButton = -1;
                        selectedButton = -1;
                    }
                    break;
                }
                }
            }
            else {
                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1) {
                    selectedButton = -1;
                    
                    for (std::size_t i(0), end(buttonPositions.size()); i != end; ++i) {
                        if (i == 0 && !factors.empty()) continue;
                        if (pointInRectInclusive(event.mouse.x, event.mouse.y, buttonPositions[i])) {
                            selectedButton = static_cast<int>(i);
                            pressedButton = selectedButton;
                            break;
                        }
                    }
                }
            }
        }
        
        if (stackClearance > 0) {
            stackClearance -= std::min(stackClearance, 1.);
        }
        
        return this;
    }
    virtual void renderTo(ALLEGRO_BITMAP* target) const {
        al_set_target_bitmap(target);
        al_clear_to_color(al_map_rgb(255, 0, 255));
    }
private:
    double stackClearance;
    int selectedButton;
    int pressedButton;
    GameSave save;
    
    DragonStats currentDeltas;
    std::vector<unsigned> factors;
    std::vector<Rect> buttonPositions;
};
#endif
