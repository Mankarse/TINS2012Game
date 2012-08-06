#ifndef TINSGame2012_MainMenu_h
#define TINSGame2012_MainMenu_h
#include <iostream>
#include "Scene.h"
#include "Globals.h"
#include "Rect.h"
#include "Game.h"
#include "GameSave.h"

class MainMenu: public Scene {

public:
    MainMenu();
    void init();
    virtual Scene* update(InputState const& input);
    virtual void renderTo(ALLEGRO_BITMAP* target) const;
private:
    std::vector<Rect> buttonPositions;
    bool saveExists;
    GameSave currentSave;
    int selectedButton;
    int pressedButton;
    ALLEGRO_SAMPLE_ID music;
};
#endif
