#include "Cave.h"
#include "Game.h"
static DragonStats getUpgradeAmounts(unsigned prime, GameSave const& save) {
    DragonStats stats;
    stats.size = 0.1 * std::pow(static_cast<double>(prime), 1.04);
    stats.wingspan = 0.1 * std::pow(static_cast<double>(prime), 1.01);
    stats.stamina = 0.1 * std::pow(static_cast<double>(prime), 1.02);
    stats.fireStrength = 0.1 * std::pow(static_cast<double>(prime), 1.01);
    stats.fireCooldown = 0.1 * std::pow(static_cast<double>(prime), 1.015);
    return stats;
}

static int const primeTileHeight = 12 * 4;

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

void Cave::nextTile() {
    stackClearance += primeTileHeight;
    factors.pop_back();
    if (!factors.empty()) {
        currentDeltas = getUpgradeAmounts(factors.back(), save);
   }
}

Cave::Cave(GameSave const& save):
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
Scene* Cave::update(InputState const& input) {
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
                                std::auto_ptr<Game> game(new Game(save));
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

void Cave::renderTo(ALLEGRO_BITMAP* target) const {
    al_set_target_bitmap(target);
    al_draw_bitmap(g_Bitmaps["CaveBackground"], 0, 0, 0);
    
    al_draw_bitmap(factors.empty() ? (pressedButton == 0 ? g_Bitmaps["CaveReturnDown"] : g_Bitmaps["CaveReturnUp"]) : g_Bitmaps["CaveReturnGrey"], 40*4, 133*4, 0);
    
    al_draw_bitmap(pressedButton == 1 ? g_Bitmaps["CaveSizeDown"] : g_Bitmaps["CaveSizeUp"], 1*4, 13*4, 0);
    al_draw_bitmap(pressedButton == 2 ? g_Bitmaps["CaveWingspanDown"] : g_Bitmaps["CaveWingspanUp"], 41*4, 13*4, 0);
    al_draw_bitmap(pressedButton == 3 ? g_Bitmaps["CaveStaminaDown"] : g_Bitmaps["CaveStaminaUp"], 81*4, 13*4, 0);
    al_draw_bitmap(pressedButton == 4 ? g_Bitmaps["CaveFlamerangeDown"] : g_Bitmaps["CaveFlamerangeUp"], 121*4, 13*4, 0);
    al_draw_bitmap(pressedButton == 5 ? g_Bitmaps["CaveFlameCooldownDown"] : g_Bitmaps["CaveFlameCooldownUp"], 161*4, 13*4, 0);
    
    drawStats();
    drawFactors();
}
void Cave::drawStat(int horizontalPosition, double value, double delta) const {
    al_draw_textf(g_Font10, al_map_rgb(255, 255, 255), horizontalPosition, 3*4, ALLEGRO_ALIGN_CENTRE, "%.0f", value);
    al_draw_textf(g_Font10, al_map_rgb(255, 255, 255), horizontalPosition, 47*4, ALLEGRO_ALIGN_CENTRE, "%+.2f", delta);
}

void Cave::drawStats() const {
    drawStat(1*4 + (37 * 4/2), save.stats.size, currentDeltas.size);
    drawStat(41*4 + (37 * 4/2), save.stats.wingspan, currentDeltas.wingspan);
    drawStat(81*4 + (37 * 4/2), save.stats.stamina, currentDeltas.stamina);
    drawStat(121*4 + (37 * 4/2), save.stats.fireStrength, currentDeltas.fireStrength);
    drawStat(161*4 + (37 * 4/2), save.stats.fireCooldown, currentDeltas.fireCooldown);
}

void Cave::drawFactors() const {
    int x(2*4);
    int y(75*4 + stackClearance);
    ALLEGRO_BITMAP* b(g_Bitmaps["CaveEmbossedTile"]);
    int width(al_get_bitmap_width(b));
    int height(al_get_bitmap_height(b));
    for (std::vector<unsigned>::const_reverse_iterator it(factors.rbegin()), end(factors.rend()); it != end; ++it) {
        al_draw_scaled_bitmap(b, 0, 0, width, height, x, y, width, height, 0);
        al_draw_textf(g_Font10, al_map_rgb(255, 255, 255), x + width/2, y + 8, ALLEGRO_ALIGN_CENTRE, "%d", *it);
        y += al_get_bitmap_height(b) + 4;
    }
}
