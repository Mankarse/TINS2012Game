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
#include <cmath>
class SimpleAnimation {
    private:
    std::vector<ALLEGRO_BITMAP*> frames;
    double timeOffset;
    double frameTime;
    
    public:
    ALLEGRO_BITMAP* getCurrentFrame(double currentTime) const{
        long curFrame((long)floor((currentTime - timeOffset) / frameTime) % (long)frames.size());
        return frames[curFrame];
    }
    ALLEGRO_BITMAP* getCurrentFrame() {
        return getCurrentFrame(al_get_time());
    }
    void setTimeOffset(double currentTime) {
        timeOffset = currentTime;
    }
    void setTimeOffset() {
        return setTimeOffset(al_get_time());
    }
    
    SimpleAnimation(double frameRate) :
    frameTime(1/frameRate),
    timeOffset(al_get_time())
    // Needs to initialise the frames in some elegant way
    {}
};

#endif
