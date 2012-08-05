#ifndef TINSGame2012_GameSave_h
#define TINSGame2012_GameSave_h
#include "DragonStats.h"
struct GameSave {
    unsigned long long totalScore;
    unsigned long long scoreDelta;
    DragonStats stats;
};

#endif
