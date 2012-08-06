#ifndef TINSGame2012_Timer_h
#define TINSGame2012_Timer_h
#include <allegro5/allegro.h>
struct Timer {
    Timer(ALLEGRO_TIMER *timer=0): timer(timer){}
    
    ALLEGRO_TIMER* get() {
        return timer;
    }
    
    ALLEGRO_TIMER* release() {
        ALLEGRO_TIMER *retV = timer;
        timer = 0;
        return retV;
    }
    
    void reset(ALLEGRO_TIMER* const timer=0) {
        al_destroy_timer(this->timer);
        this->timer = timer;
    }
    
    ~Timer() {
        al_destroy_timer(timer);
    }
private:
    Timer(Timer const&);
    Timer& operator=(Timer const&);
    ALLEGRO_TIMER *timer;
};
#endif
