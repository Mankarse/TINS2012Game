#include <stdio.h>
#include <allegro5/allegro.h>
#include <memory>
 
int main(int argc, char **argv){
    
    ALLEGRO_DISPLAY *display = NULL;
    
    if(!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }
    
    display = al_create_display(640, 480);
    if(!display) {
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }
    
    al_clear_to_color(al_map_rgb(0,0,0));
    
    al_flip_display();
    
    al_rest(10.0);
    
    al_destroy_display(display);
    
    return 0;
}

int const fps(60);

void mainLoop(std::auto_ptr<Screen> screen) {
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / fps);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(g_display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    LagLimiter lag;
    InputState input;
    while (currentScreen) {
        //readInput reads until the timer triggers
        //so it is what regulates game speed.
        input.updateState(event_queue);
        std::auto_ptr<Scene> newScreen(currentScreen.update(input));
        if (newScreen != currentScreen) {
            currentScreen = newScreen;
            lag.reset();
            continue;
        }
        
        if (!lag.fallingBehind()) {
            currentScreen.renderTo(backbuffer);
            //flip display
        }
    }
}

