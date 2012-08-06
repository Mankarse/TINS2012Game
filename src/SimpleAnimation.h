#ifndef TINSGame2012_SimpleAnimation_h
#define TINSGame2012_SimpleAnimation_h

#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include "Filesystem.h"

static std::vector<ALLEGRO_BITMAP*> loadFramesFromFilename(std::string namePrefix)
{
    std::vector<ALLEGRO_BITMAP*> retV;
    int curIndex(0);
    std::stringstream formatter;
    formatter << namePrefix;
    formatter << curIndex++;
    while (g_Bitmaps.find(formatter.str()) != g_Bitmaps.end())
    {
        retV.push_back(g_Bitmaps[formatter.str()]);
        formatter.str("");
        formatter << namePrefix;
        formatter << curIndex++;
    }
    return retV;
}
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
    ALLEGRO_BITMAP* getCurrentFrame() const {
        return getCurrentFrame(al_get_time());
    }
    void setTimeOffset(double currentTime) {
        timeOffset = currentTime;
    }
    void setTimeOffset() {
        return setTimeOffset(al_get_time());
    }
    
    SimpleAnimation(double frameRate, std::string fileName) :
    frameTime(1./frameRate),
    timeOffset(al_get_time()),
    frames(loadFramesFromFilename(fileName))
    {}
};

#endif
