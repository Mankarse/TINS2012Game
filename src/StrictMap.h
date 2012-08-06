#ifndef TINSGame2012_StrictMap_h
#define TINSGame2012_StrictMap_h
#include <map>
#include <cassert>
template <typename K, typename V>
struct StrictMap : std::map<K,V>{
    typedef std::map<K,V> base_t;
    V& operator[](K const& k) {
        assert(base_t::find(k) != base_t::end());
        return base_t::operator[](k);
    }
};
#endif
