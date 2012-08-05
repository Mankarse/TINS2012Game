#ifndef TINSGame2012_GameSave_h
#define TINSGame2012_GameSave_h
#include <string>
#include <utility>
#include "DragonStats.h"
struct GameSave {
    unsigned long long totalScore;
    unsigned long long scoreDelta;
    DragonStats stats;
};

std::string obfuscate(std::string const& str);
std::string deobfuscate(std::string const& str);
std::string serialize(GameSave const& save);
std::pair<bool, GameSave> deserialize(std::string const& save);
std::pair<bool, GameSave> loadGame();
void saveGame(GameSave const& game);
#endif
