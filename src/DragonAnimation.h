#ifndef TINSGame2012_DragonAnimation_h
#define TINSGame2012_DragonAnimation_h

#include <vector>
#include <cmath>
#include "Geometry.h"
/*
struct DragonFrame {
    ALLEGRO_BITMAP* frame;
    Point2D pixelOffset;
    DragonFrame(ALLEGRO_BITMAP* image, Point2D pos) :
    frame(image),
    pixelOffset(pos)
    {}
    
}
static std::vector<DragonFrame> loadDragonFramesFromFilename(std::string namePrefix)
{
    std::vector<DragonFrame> retV;
    int curIndex(0);
    std::stringstream formatter;
    formatter << namePrefix;
    formatter << curIndex++;
    while (g_Bitmaps.find(formatter.str()) != g_Bitmaps.end())
    {
        retV.push_back(DragonFrame(g_Bitmaps[formatter.str()], ));
        formatter << namePrefix;
        formatter << curIndex++;
    }
    return retV;
}*/

class DragonAnimation {
    private:
    std::vector<DragonFrame> frames;
    double timeOffset;
    double frameTime;
    
    public:
    DragonFrame getCurrentFrame(double currentTime) const{
        long curFrame((long)floor((currentTime - timeOffset) / frameTime) % (long)frames.size());
        return frames[curFrame];
    }
    DragonFrame getCurrentFrame() {
        return getCurrentFrame(al_get_time());
    }
    void setTimeOffset(double currentTime) {
        timeOffset = currentTime;
    }
    void setTimeOffset() {
        return setTimeOffset(al_get_time());
    }
    
    DragonAnimation(double frameRate) :
    frameTime(1/frameRate),
    timeOffset(al_get_time())
    // Needs to initialise the frames in some elegant way
    {}
};

#endif
