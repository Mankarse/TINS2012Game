#ifndef TINSGame2012_InputState_h
#define TINSGame2012_InputState_h
#include <vector>
#include <allegro5/allegro.h>
struct InputState {
    InputState(ALLEGRO_TIMER* timer):timer(timer) {}
    void updateState(ALLEGRO_EVENT_QUEUE* eventQueue) {
        events.clear();
        while (true) {
            ALLEGRO_EVENT event;
            al_wait_for_event(eventQueue, &event);
            if (event.type == ALLEGRO_EVENT_TIMER && event.timer.source == timer) {
                al_get_keyboard_state(&keyboardState);
                al_get_mouse_state(&mouseState);
                break;
            }
            events.push_back(event);
        }
    }
    
    std::vector<ALLEGRO_EVENT> events;
    ALLEGRO_KEYBOARD_STATE keyboardState;
    ALLEGRO_MOUSE_STATE mouseState;
    
    private:
    ALLEGRO_TIMER* timer;
};
#endif
