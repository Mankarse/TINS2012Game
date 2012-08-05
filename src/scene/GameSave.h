#ifndef TINSGame2012_GameSave_h
#define TINSGame2012_GameSave_h
#include <string>
#include <sstream>
#include "DragonStats.h"
struct GameSave {
    unsigned long long totalScore;
    unsigned long long scoreDelta;
    DragonStats stats;
};

inline std::string obfuscate(std::string const&) {
    
}

inline std::string deobfuscate(std::string const&) {
    
}

inline std::string save(GameSave const& save) {
    std::stringstream formatter;
    formatter
        << save.totalScore << ' '
        << save.scoreDelta << ' '
        << save.stats.size << ' '
        << save.stats.wingspan << ' '
        << save.stats.stamina << ' '
        << save.stats.fireStrength << ' '
        << save.stats.fireCooldown;
    return obfuscate(formatter.str());
}

inline GameSave load(std::string const& save) {
    std::string str(deobfuscate(save));
    
}

#endif
