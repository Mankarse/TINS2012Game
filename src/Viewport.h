//
//  Viewport.h
//  TINSGame2012
//
//  Created by Evan Wallace on 4/08/12.
//
//

#ifndef TINSGame2012__Viewport
#define TINSGame2012__Viewport
#include <allegro5/allegro.h>
#include <vector>
class Viewport {
    private:
    ALLEGRO_TRANSFORM myTrans;
    std::vector<ALLEGRO_BITMAP> images;
    
    public:
    void renderScreen();
};

#endif /* defined(__TINSGame2012__Viewport__) */
