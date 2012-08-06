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
    currentSave(-1,-1,-1,DragonStats(-1,-1,-1,-1,-1)),
    selectedButton(-1),
    pressedButton(-1)
{}
void MainMenu::init() {
    al_play_sample(g_Samples["MenuMus"], 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, &music);
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
                        case 2: {
                            std::auto_ptr<Cave> cave(new Cave(currentSave));
                            al_stop_sample(&music);
                            return cave.release();
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

void MainMenu::renderTo(ALLEGRO_BITMAP* target) const {
    al_set_target_bitmap(target);
    
    al_draw_bitmap(g_Bitmaps["MenuBackground"], 0, 0, 0);
    al_draw_bitmap(g_Bitmaps["MenuTitle"], 0, 0, 0);
    
    //al_draw_bitmap(pressedButton == 0 ? g_Bitmaps["MenuInstructionsDown"] : g_Bitmaps["MenuInstructionsUp"], 60*4, 47*4, 0);
    al_draw_bitmap(pressedButton == 1 ? g_Bitmaps["MenuNewGameDown"] : g_Bitmaps["MenuNewGameUp"], 63*4, 73*4, 0);
    al_draw_bitmap(saveExists ? (pressedButton == 2 ? g_Bitmaps["MenuContinueDown"] : g_Bitmaps["MenuContinueUp"]) : g_Bitmaps["MenuContinueGrey"], 47*4, 100*4, 0);
    al_draw_bitmap(pressedButton == 3 ? g_Bitmaps["MenuExitDown"] : g_Bitmaps["MenuExitUp"], 85*4, 126*4, 0);
}

