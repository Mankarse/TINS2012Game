//
//  SimpleAnimation.h
//  TINSGame2012
//
//  Created by Evan Wallace on 6/08/12.
//
//

#ifndef TINSGame2012_SimpleAnimation_h
#define TINSGame2012_SimpleAnimation_h

#include <vector>

class SimpleAnimation {
    private:
    std::vector<ALLEGRO_BITMAP*> frames;
    double timeOffset;
    double frameTime;
    
    public:
    ALLEGRO_BITMAP* getCurrentFrame(double currentTime) {
        
    }
    ALLEGRO_BITMAP* getCurrentFrame() {
        return getCurrentFrame(al_get_time());
    }
};

#endif
