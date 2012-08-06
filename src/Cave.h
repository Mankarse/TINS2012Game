#ifndef TINSGame2012__Cave
#define TINSGame2012__Cave
#include "Scene.h"
#include "GameSave.h"

#include <allegro5/allegro.h>
#include <cmath>
#include <algorithm>
#include "Rect.h"
#include "Primes.h"


class Cave: public Scene {
public:
    Cave(GameSave const& save);
    
    virtual Scene* update(InputState const& input);
    virtual void renderTo(ALLEGRO_BITMAP* target) const;

private:
    void nextTile();
    
    void drawStat(int horizontalPosition, double value, double delta) const;
    void drawStats() const;
    void drawFactors() const;
    
    double stackClearance;
    int selectedButton;
    int pressedButton;
    GameSave save;
    
    DragonStats currentDeltas;
    std::vector<unsigned> factors;
    std::vector<Rect> buttonPositions;
};
#endif
