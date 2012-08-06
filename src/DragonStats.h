#ifndef TINSGame2012_DragonStats_h
#define TINSGame2012_DragonStats_h

struct DragonStats{
    DragonStats() :
        size(30),
        wingspan(5),
        stamina(40),
        fireStrength(500),
        fireCooldown(10)
    {
    }
    DragonStats(double size, double wingspan, double stamina, double fireStrength, double fireCooldown) :
        size(size), wingspan(wingspan), stamina(stamina), fireStrength(fireStrength), fireCooldown(fireCooldown)
    {
    }
    double size;
    double wingspan;
    double stamina;
    double fireStrength;
    double fireCooldown;
};

#endif
