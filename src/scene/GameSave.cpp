#include "GameSave.h"
#include <sstream>
#include <fstream>
#include <vector>

namespace {
    template<typename T, std::size_t N>
    std::size_t length(T(&)[N]) {
        return N;
    }
}


int key[] = {
    14, 2, 5, 9, 12, 27, 16, 32, 43, 12, 9, 38,
};

std::string obfuscate(std::string const& str) {
    std::string retv(str);
    std::size_t key_pos(0);
    for (std::string::iterator it(retv.begin()), end(retv.end()); it != end; ++it) {
        if (key_pos == length(key)) key_pos = 0;
        
        *it += key[key_pos];
        
        ++key_pos;
    }
    return retv;
}

std::string deobfuscate(std::string const& str) {
    std::string retv(str);
    std::size_t key_pos(0);
    for (std::string::iterator it(retv.begin()), end(retv.end()); it != end; ++it) {
        if (key_pos == length(key)) key_pos = 0;
        
        *it -= key[key_pos];
        
        ++key_pos;
    }
    return retv;
}

std::string serialize(GameSave const& save) {
    std::stringstream formatter;
    formatter
        << save.totalScore << ' '
        << save.scoreDelta << ' '
        << save.totalPlayTime << ' '
        << save.stats.size << ' '
        << save.stats.wingspan << ' '
        << save.stats.stamina << ' '
        << save.stats.fireStrength << ' '
        << save.stats.fireCooldown;
    return formatter.str();
}

std::pair<bool, GameSave> deserialize(std::string const& save) {
    std::stringstream reader(save);
    GameSave retV(0,0,0,DragonStats(0,0,0,0,0));
    reader
        >> retV.totalScore
        >> retV.scoreDelta
        >> retV.totalPlayTime
        >> retV.stats.size
        >> retV.stats.wingspan
        >> retV.stats.stamina
        >> retV.stats.fireStrength
        >> retV.stats.fireCooldown;
    return std::make_pair(!reader.fail(), retV);
}

std::pair<bool, GameSave> loadGame() {
    GameSave retV(0,0,0,DragonStats(0,0,0,0,0));
    std::vector<char> vec;
    std::ifstream file;
    file.open("savegame.sav");
    if (!file.is_open()) {
        return std::make_pair(false, retV);
    }
    file.seekg(0, std::ios::end);
    std::streampos length(file.tellg());
    if (length) {
        file.seekg(0, std::ios::beg);
        vec.resize(static_cast<std::size_t>(length));
        file.read(&vec.front(), static_cast<std::size_t>(length));
    }
    if (!file) {
        return std::make_pair(false, retV);
    }
    
    return deserialize(deobfuscate(std::string(vec.begin(), vec.end())));
}

void saveGame(GameSave const& game) {
    std::ofstream file;
    file.open("savegame.sav");
    file << obfuscate(serialize(game));
}
