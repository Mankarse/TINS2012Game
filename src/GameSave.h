#ifndef TINSGame2012_GameSave_h
#define TINSGame2012_GameSave_h
#include <string>
#include <utility>
#include "DragonStats.h"
struct GameSave {
    GameSave(unsigned totalScore, unsigned scoreDelta, unsigned totalPlayTime, DragonStats const& stats) :
        totalScore(totalScore), scoreDelta(scoreDelta), totalPlayTime(totalPlayTime), stats(stats)
    {}
    unsigned totalScore;
    unsigned scoreDelta;
    double totalPlayTime;
    DragonStats stats;
};

std::string obfuscate(std::string const& str);
std::string deobfuscate(std::string const& str);
std::string serialize(GameSave const& save);
std::pair<bool, GameSave> deserialize(std::string const& save);
std::pair<bool, GameSave> loadGame();
void saveGame(GameSave const& game);
#endif
