#ifndef TINSGame2012_DragonStats_h
#define TINSGame2012_DragonStats_h

struct DragonStats{
    DragonStats() :
        size(30),
        wingspan(5),
        stamina(40),
        fireStrength(100),
        fireCooldown(100)
    {
    }
    double size;
    double wingspan;
    double stamina;
    double fireStrength;
    double fireCooldown;
};

#endif
