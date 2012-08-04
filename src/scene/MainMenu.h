#ifndef TINSGame2012_MainMenu_h
#define TINSGame2012_MainMenu_h
#include <iostream>
#include "Scene.h"
#include "Globals.h"
#include "Rect.h"
#include "Game.h"

inline bool isDisplayClosedEvent(ALLEGRO_EVENT const& e) {
    return e.type == ALLEGRO_EVENT_DISPLAY_CLOSE;
}

class MainMenu: public Scene {
public:
    MainMenu() :
        saveExists(false),
        selectedButton(-1),
        pressedButton(-1)
    {}
    void init() {
        al_play_sample(g_TitleMus, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, &music);
    }
    virtual Scene* update(InputState const& input) {
        if (std::find_if(
                input.events.begin(), input.events.end(), isDisplayClosedEvent)
            != input.events.end())
        {
            al_stop_sample(&music);
            return 0;
        }
        
        for (std::vector<ALLEGRO_EVENT>::const_iterator it(input.events.begin()), end(input.events.end()); it != end; ++it)
        {
            ALLEGRO_EVENT const& event(*it);
            if (selectedButton == -1) {
                if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
                    
                }
                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                    
                }
            }
            else {
                
            }
        }
        
        
        return this;
    }
    
    virtual void renderTo(ALLEGRO_BITMAP* target) const {
        al_set_target_bitmap(target);
        
        draw_element(g_MenuBackground);
        draw_element(g_Title);
        
        
        draw_element(pressedButton == 0 ? g_InstructionsDown : g_InstructionsUp);
        draw_element(pressedButton == 1 ? g_NewGameDown : g_NewGameUp);
        draw_element(saveExists ? (pressedButton == 3 ? g_ContinueDown : g_ContinueUp) : g_ContinueGrey);
        draw_element(pressedButton == 4 ? g_ExitDown : g_ExitUp);
    }
private:
    std::vector<Rect> buttonPositions;
    bool saveExists;
    int selectedButton;
    int pressedButton;
    ALLEGRO_SAMPLE_ID music;
    
    static void draw_element(ALLEGRO_BITMAP * element) {
        al_draw_scaled_bitmap(
            element,
            0, 0, al_get_bitmap_width(element), al_get_bitmap_height(element),
            0, 0, al_get_bitmap_width(element)*4, al_get_bitmap_height(element)*4,
            0);
    }
};
#endif
