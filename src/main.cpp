#include "Globals.h"

#include <iostream>
#include <exception>
#include <allegro5/allegro.h>
#include <memory>
#include <allegro5/allegro_image.h>

ALLEGRO_DISPLAY *g_display;
ALLEGRO_BITMAP *g_ContinueDown;
ALLEGRO_BITMAP *g_ContinueGrey;
ALLEGRO_BITMAP *g_ContinueUp;
ALLEGRO_BITMAP *g_ExitDown;
ALLEGRO_BITMAP *g_ExitUp;
ALLEGRO_BITMAP *g_InstructionsDown;
ALLEGRO_BITMAP *g_InstructionsUp;
ALLEGRO_BITMAP *g_MenuBackground;
ALLEGRO_BITMAP *g_NewGameDown;
ALLEGRO_BITMAP *g_NewGameUp;
ALLEGRO_BITMAP *g_Title;


struct LoadingException : std::exception {
    LoadingException(char const* what):what_message(what){}
    char const *what() const throw(){return what_message;}
    char const* what_message;
};

struct CoreAllegroInit {
    CoreAllegroInit() {
        if(!al_init()) {
            throw LoadingException("Failed to initialize allegro!\n");
        }
    }
};

struct DisplayInit {
    ALLEGRO_DISPLAY *display;
    DisplayInit() :
        display(al_create_display(800, 600))
    {
        if(!display) {
            throw LoadingException("failed to create display!\n");
        }
        g_display = display;
    }
    ~DisplayInit() {
        g_display = 0;
        al_destroy_display(display);
    }
};

struct ImageAddonInit {

};

struct AllegroInit {
    CoreAllegroInit coreAllegro;
    ImageAddonInit imageAddon;
};

struct ResourcesInit {
    ResourcesInit()
    {
        
    }
    ~ResourcesInit() {}
};

struct Initializer {
    AllegroInit allegroInit;
    DisplayInit displayInit;
    ResourcesInit resourcesInit;
};

int main(int argc, char **argv){
    try {
        Initializer init;

        al_clear_to_color(al_map_rgb(0,0,0));
        
        al_flip_display();
        
        al_rest(10.0);

        return 0;
    } catch (std::exception const& e) {
        std::cout << "Failed:\n";
        std::cout << e.what();
    }
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

