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

static int const primeTileHeight = 12 * 4;

class Cave: public Scene {
    static bool isDisplayClosedEvent(ALLEGRO_EVENT const& e) {
        return e.type == ALLEGRO_EVENT_DISPLAY_CLOSE;
    }
    static std::vector<Rect> loadButtonPositions() {
        std::vector<Rect> retv;
        retv.push_back(Rect(40, 133, 159, 16)*4);
        retv.push_back(Rect(1, 13, 37, 31)*4);
        retv.push_back(Rect(41, 13, 37, 31)*4);
        retv.push_back(Rect(81, 13, 37, 31)*4);
        retv.push_back(Rect(121, 13, 37, 31)*4);
        retv.push_back(Rect(161, 13, 37, 31)*4);
        return retv;
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
        selectedButton(-1),
        pressedButton(-1),
        save(save),
        factors(factorize(save.scoreDelta)),
        buttonPositions(loadButtonPositions())
    {
        if (!factors.empty()) currentDeltas = getUpgradeAmounts(factors.back(), save);
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
                                case 1: if(!factors.empty()){save.stats.size += currentDeltas.size; nextTile();} break;
                                case 2: if(!factors.empty()){save.stats.wingspan += currentDeltas.wingspan; nextTile();} break;
                                case 3: if(!factors.empty()){save.stats.stamina += currentDeltas.stamina; nextTile();} break;
                                case 4: if(!factors.empty()){save.stats.fireStrength += currentDeltas.fireStrength; nextTile();} break;
                                case 5: if(!factors.empty()){save.stats.fireCooldown += currentDeltas.fireCooldown; nextTile();} break;
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
                        if (i != 0 && factors.empty()) continue;
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
            stackClearance -= std::min(stackClearance, 5.);
        }
        
        return this;
    }

    virtual void renderTo(ALLEGRO_BITMAP* target) const {
        al_set_target_bitmap(target);
        draw_element(g_CaveBackground);
        
        draw_element(factors.empty() ? (pressedButton == 0 ? g_CaveReturnDown : g_CaveReturnUp) : g_CaveReturnGrey);
        
        draw_element(pressedButton == 1 ? g_CaveSizeDown : g_CaveSizeUp);
        
        draw_element(pressedButton == 2 ? g_CaveWingspanDown : g_CaveWingspanUp);
        draw_element(pressedButton == 3 ? g_CaveStaminaDown : g_CaveStaminaUp);
        draw_element(pressedButton == 4 ? g_CaveFlamerangeDown : g_CaveFlamerangeUp);
        draw_element(pressedButton == 5 ? g_CaveFlameCooldownDown : g_CaveFlameCooldownUp);
        
        drawStats();
        drawFactors();
    }
    void drawStats() const {
        al_draw_text(g_Font10, al_map_rgb(255, 0, 255), 100, 100, ALLEGRO_ALIGN_CENTRE, "14");
    }
    
    void drawFactors() const {
        int x(2*4);
        int y(75*4 + stackClearance);
        ALLEGRO_BITMAP* b(g_CaveEmbossedTile);
        for (std::vector<unsigned>::const_reverse_iterator it(factors.rbegin()), end(factors.rend()); it != end; ++it) {
            al_draw_scaled_bitmap(b, 0, 0, al_get_bitmap_width(b), al_get_bitmap_height(b), x, y, al_get_bitmap_width(b), al_get_bitmap_height(b), 0);
            y += al_get_bitmap_height(b) + 4;
        }
    }
    
private:
    static void draw_element(ALLEGRO_BITMAP * element) {
        al_draw_scaled_bitmap(
            element,
            0, 0, al_get_bitmap_width(element), al_get_bitmap_height(element),
            0, 0, al_get_bitmap_width(element), al_get_bitmap_height(element),
            0);
    }

    double stackClearance;
    int selectedButton;
    int pressedButton;
    GameSave save;
    
    DragonStats currentDeltas;
    std::vector<unsigned> factors;
    std::vector<Rect> buttonPositions;
};
#endif
