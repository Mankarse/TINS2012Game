#include "Globals.h"

#include <iostream>
#include <exception>
#include <allegro5/allegro.h>
#include <memory>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "scene/MainMenu.h"

ALLEGRO_DISPLAY *g_display;

ALLEGRO_BITMAP *g_ContinueDown;
ALLEGRO_BITMAP *g_ContinueGrey;
ALLEGRO_BITMAP *g_ContinueUp;
ALLEGRO_BITMAP *g_ExitDown;
ALLEGRO_BITMAP *g_ExitUp;
ALLEGRO_BITMAP *g_LevelFG;
ALLEGRO_BITMAP *g_InstructionsDown;
ALLEGRO_BITMAP *g_InstructionsUp;
ALLEGRO_BITMAP *g_MenuBackground;
ALLEGRO_BITMAP *g_NewGameDown;
ALLEGRO_BITMAP *g_NewGameUp;
ALLEGRO_BITMAP *g_DragonSmallStand;
ALLEGRO_BITMAP *g_Title;


ALLEGRO_SAMPLE *g_TitleMus;

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
        display((al_set_new_display_option(ALLEGRO_VSYNC,1,ALLEGRO_SUGGEST), al_create_display(800, 600)))
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

struct KeyboardInit {
    KeyboardInit(){
        if(!al_install_keyboard()) {
            throw LoadingException("failed to install keyboard!\n");
        }
    }
};
struct MouseInit {
    MouseInit(){
        if(!al_install_mouse()) {
            throw LoadingException("failed to install mouse!\n");
        }
    }
};
struct ImageAddonInit {
    ImageAddonInit() {
       if (!al_init_image_addon()){
           throw LoadingException("failed to initialize image I/O addon\n");
       }
    }
};

struct AudioInit {
    AudioInit() {
        if(!al_install_audio()) {
            throw LoadingException("failed to initialize audio addon\n");
        }
    }
};

struct AudioSamplesInit {
    AudioSamplesInit() {
        if(!al_reserve_samples(128)) {
            throw LoadingException("failed to reserve samples\n");
        }
    }
};

struct AudioCodecInit {
    AudioCodecInit() {
        if (!al_init_acodec_addon()) {
            throw LoadingException("failed to initialize audio codec addon\n");
        }
    }
};


struct AllegroInit {
    CoreAllegroInit coreAllegro;
    KeyboardInit keyboard;
    MouseInit mouse;
    ImageAddonInit imageAddon;
    AudioInit audio;
    AudioSamplesInit samples;
    AudioCodecInit acodecs;
};

struct BitmapInit {
    BitmapInit(ALLEGRO_BITMAP **toInitialize, char const* filename):
        toInitialize(toInitialize)
    {
        *toInitialize = al_load_bitmap(filename);
        if (!*toInitialize) {
            throw LoadingException("Failed to load bitmap\n");
        }
    }
    ~BitmapInit() {
        al_destroy_bitmap(*toInitialize);
    }
    private:
    ALLEGRO_BITMAP **toInitialize;
};


struct SampleInit {
    SampleInit(ALLEGRO_SAMPLE **toInitialize, char const* filename):
        toInitialize(toInitialize)
    {
        *toInitialize = al_load_sample(filename);
        if (!*toInitialize) {
            throw LoadingException("Failed to load sample\n");
        }
    }
    ~SampleInit() {
        al_destroy_sample(*toInitialize);
    }
    private:
    ALLEGRO_SAMPLE **toInitialize;
};

struct ResourcesInit {
    ResourcesInit() :
        ContinueDown(&g_ContinueDown, "ContinueDown.png"),
        ContinueGrey(&g_ContinueGrey, "ContinueGrey.png"),
        ContinueUp(&g_ContinueUp, "ContinueUp.png"),
        ExitDown(&g_ExitDown, "ExitDown.png"),
        ExitUp(&g_ExitUp, "ExitUp.png"),
        LevelFG(&g_LevelFG, "LevelFG.png"),
        InstructionsDown(&g_InstructionsDown, "InstructionsDown.png"),
        InstructionsUp(&g_InstructionsUp, "InstructionsUp.png"),
        MenuBackground(&g_MenuBackground, "MenuBackground.png"),
        NewGameDown(&g_NewGameDown, "NewGameDown.png"),
        NewGameUp(&g_NewGameUp, "NewGameUp.png"),
        DragonSmallStand (&g_DragonSmallStand, "DragonSmallStand.png"),
        Title(&g_Title, "Title.png"),
        TitleMus(&g_TitleMus, "TitleMus.ogg")
    {
    }
    ~ResourcesInit() {}
    BitmapInit ContinueDown;
    BitmapInit ContinueGrey;
    BitmapInit ContinueUp;
    BitmapInit ExitDown;
    BitmapInit ExitUp;
    BitmapInit LevelFG;
    BitmapInit InstructionsDown;
    BitmapInit InstructionsUp;
    BitmapInit MenuBackground;
    BitmapInit NewGameDown;
    BitmapInit NewGameUp;
    BitmapInit DragonSmallStand;
    BitmapInit Title;
    
    SampleInit TitleMus;
};

struct Initializer {
    AllegroInit allegroInit;
    DisplayInit displayInit;
    ResourcesInit resourcesInit;
};

void mainLoop(std::auto_ptr<Scene> screen);

int main(int argc, char **argv){
    try {
        Initializer init;
        
        std::auto_ptr<MainMenu> mainMenu(new MainMenu());
        mainMenu->init();
        mainLoop(std::auto_ptr<Scene>(mainMenu.release()));
        
        return 0;
    } catch (std::exception const& e) {
        std::cout << "Failed:\n";
        std::cout << e.what();
        return -1;
    }
}

int const fps(60);

void mainLoop(std::auto_ptr<Scene> scene) {
    ALLEGRO_TIMER *timer(al_create_timer(1.0 / fps));
    ALLEGRO_EVENT_QUEUE *event_queue(al_create_event_queue());
    al_register_event_source(event_queue, al_get_display_event_source(g_display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_start_timer(timer);

    //LagLimiter lag;
    InputState input(timer);
    while (scene.get()) {
        //readInput reads until the timer triggers
        //so it is what regulates game speed.
        input.updateState(event_queue);
        Scene* newScene(scene->update(input));
        if (newScene != scene.get()) {
            scene.reset(newScene);
            //lag.reset();
            continue;
        }
        
        //if (!lag.fallingBehind()) {
            scene->renderTo(al_get_backbuffer(g_display));
            al_flip_display();
        //}
    }
}

