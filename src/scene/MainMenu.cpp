#include "MainMenu.h"
#include "Game.h"

static bool isDisplayClosedEvent(ALLEGRO_EVENT const& e) {
    return e.type == ALLEGRO_EVENT_DISPLAY_CLOSE;
}

static std::vector<Rect> loadButtonPositions() {
    std::vector<Rect> buttonPositions;
    buttonPositions.push_back(Rect(60, 47, 77, 15)*4);
    buttonPositions.push_back(Rect(63, 73, 70, 17)*4);
    buttonPositions.push_back(Rect(47, 100, 102, 17)*4);
    buttonPositions.push_back(Rect(85, 126, 28, 17)*4);
    return buttonPositions;
}

MainMenu::MainMenu() :
    buttonPositions(loadButtonPositions()),
    saveExists(false),
    selectedButton(-1),
    pressedButton(-1)
{}
void MainMenu::init() {
    al_play_sample(g_MenuMus, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, &music);
}
Scene* MainMenu::update(InputState const& input) {
    if (std::find_if(
            input.events.begin(), input.events.end(), isDisplayClosedEvent)
        != input.events.end())
    {
        al_stop_sample(&music);
        return 0;
    }
    std::pair<bool, GameSave> loadedSave(loadGame());
    saveExists = loadedSave.first;
    if (loadedSave.first) {
        currentSave = loadedSave.second;
    }
    for (std::vector<ALLEGRO_EVENT>::const_iterator it(input.events.begin()), end(input.events.end()); it != end; ++it)
    {
        ALLEGRO_EVENT const& event(*it);
        if (selectedButton != -1) {
            if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
                pressedButton = pointInRectInclusive(event.mouse.x, event.mouse.y, buttonPositions[selectedButton]) ? selectedButton : -1;
            }
            if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && event.mouse.button == 1) {
                if (pointInRectInclusive(event.mouse.x, event.mouse.y, buttonPositions[selectedButton])) {
                    switch (selectedButton) {
                        case 1: {
                            std::auto_ptr<Game> game(new Game());
                            game->init();
                            al_stop_sample(&music);
                            return game.release();
                        }
                        case 3: {
                            al_stop_sample(&music);
                            return 0;
                        }
                    }
                }
                pressedButton = -1;
                selectedButton = -1;
            }
        }
        else {
            if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1) {
                selectedButton = -1;
                
                for (std::size_t i(0), end(buttonPositions.size()); i != end; ++i) {
                    if (i == 2 && !saveExists) continue;
                    if (pointInRectInclusive(event.mouse.x, event.mouse.y, buttonPositions[i])) {
                        selectedButton = static_cast<int>(i);
                        pressedButton = selectedButton;
                        break;
                    }
                }
            }
        }
    }
    
    return this;
}

static void draw_element(ALLEGRO_BITMAP * element) {
    al_draw_scaled_bitmap(
        element,
        0, 0, al_get_bitmap_width(element), al_get_bitmap_height(element),
        0, 0, al_get_bitmap_width(element)*4, al_get_bitmap_height(element)*4,
        0);
}

void MainMenu::renderTo(ALLEGRO_BITMAP* target) const {
    al_set_target_bitmap(target);
    
    draw_element(g_MenuBackground);
    draw_element(g_MenuTitle);
    
    draw_element(pressedButton == 0 ? g_MenuInstructionsDown : g_MenuInstructionsUp);
    draw_element(pressedButton == 1 ? g_MenuNewGameDown : g_MenuNewGameUp);
    draw_element(saveExists ? (pressedButton == 2 ? g_MenuContinueDown : g_MenuContinueUp) : g_MenuContinueGrey);
    draw_element(pressedButton == 3 ? g_MenuExitDown : g_MenuExitUp);
}

