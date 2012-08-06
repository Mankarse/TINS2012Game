#ifndef TINSGame2012_EventQueue_h
#define TINSGame2012_EventQueue_h
#include <allegro5/allegro.h>
struct EventQueue {
    EventQueue(ALLEGRO_EVENT_QUEUE *eventQueue=0): eventQueue(eventQueue){}
    
    ALLEGRO_EVENT_QUEUE* get() {
        return eventQueue;
    }
    
    ALLEGRO_EVENT_QUEUE* release() {
        ALLEGRO_EVENT_QUEUE *retV = eventQueue;
        eventQueue = 0;
        return retV;
    }
    
    void reset(ALLEGRO_EVENT_QUEUE* const eventQueue=0) {
        al_destroy_event_queue(this->eventQueue);
        this->eventQueue = eventQueue;
    }
    
    ~EventQueue() {
        al_destroy_event_queue(eventQueue);
    }
private:
    EventQueue(EventQueue const&);
    EventQueue& operator=(EventQueue const&);
    ALLEGRO_EVENT_QUEUE *eventQueue;
};
#endif
