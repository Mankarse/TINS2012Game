#include "Globals.h"

#include <iostream>
#include <exception>
#include <allegro5/allegro.h>
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
        display(al_create_display(640, 480))
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

//Load all libraries and resources.
//Returns false on failure, true on success.
//Prints to stderr the cause for failure.
bool init() {
    
    if (!al_init_image_addon()) {
        std::cerr << "failed to initialize audio addon!\n";
        return false;
    }
    return true;
}

int main(int argc, char **argv){
    try {
        Initializer init;
        
        ALLEGRO_DISPLAY *display = NULL;
        
        display = al_create_display(640, 480);
        if(!display) {
            fprintf(stderr, "failed to create display!\n");
            return -1;
        }
        
        al_clear_to_color(al_map_rgb(0,0,0));
        
        al_flip_display();
        
        al_rest(10.0);

        return 0;
    } catch (std::exception const& e) {
        std::cout << "Failed:\n";
        std::cout << e.what();
    }
}