#include "Globals.h"

#include <iostream>
#include <exception>
#include <allegro5/allegro.h>
#include <memory>
#include <cstdlib>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include "scene/MainMenu.h"
#include "scene/Cave.h"
#include "Bitmap.h"

ALLEGRO_DISPLAY *g_display;

ALLEGRO_BITMAP *g_CaveBackground;
ALLEGRO_BITMAP *g_CaveEmbossedTile;
ALLEGRO_BITMAP *g_CaveFlameCooldownDown;
ALLEGRO_BITMAP *g_CaveFlameCooldownUp;
ALLEGRO_BITMAP *g_CaveFlamerangeDown;
ALLEGRO_BITMAP *g_CaveFlamerangeUp;
ALLEGRO_BITMAP *g_CaveReturnDown;
ALLEGRO_BITMAP *g_CaveReturnGrey;
ALLEGRO_BITMAP *g_CaveReturnUp;
ALLEGRO_BITMAP *g_CaveSizeDown;
ALLEGRO_BITMAP *g_CaveSizeUp;
ALLEGRO_BITMAP *g_CaveStaminaDown;
ALLEGRO_BITMAP *g_CaveStaminaUp;
ALLEGRO_BITMAP *g_CaveWingspanDown;
ALLEGRO_BITMAP *g_CaveWingspanUp;
ALLEGRO_BITMAP *g_DragonSmallStand;
ALLEGRO_BITMAP *g_LevelFG;
ALLEGRO_BITMAP *g_LevelSky;
ALLEGRO_BITMAP *g_GameUI;
ALLEGRO_BITMAP *g_MenuBackground;
ALLEGRO_BITMAP *g_MenuContinueDown;
ALLEGRO_BITMAP *g_MenuContinueGrey;
ALLEGRO_BITMAP *g_MenuContinueUp;
ALLEGRO_BITMAP *g_MenuExitDown;
ALLEGRO_BITMAP *g_MenuExitUp;
ALLEGRO_BITMAP *g_MenuInstructionsDown;
ALLEGRO_BITMAP *g_MenuInstructionsUp;
ALLEGRO_BITMAP *g_MenuNewGameDown;
ALLEGRO_BITMAP *g_MenuNewGameUp;
ALLEGRO_BITMAP *g_MenuTitle;
ALLEGRO_BITMAP *g_NumberSheet10;

ALLEGRO_FONT *g_Font10;

ALLEGRO_SAMPLE *g_MenuMus;
ALLEGRO_SAMPLE *g_DragonRoar;
ALLEGRO_SAMPLE *g_FireLoop;
ALLEGRO_SAMPLE *g_Footstep1;
ALLEGRO_SAMPLE *g_Footstep2;
ALLEGRO_SAMPLE *g_Footstep3;
ALLEGRO_SAMPLE *g_Footstep4;
ALLEGRO_SAMPLE *g_GameMus;
ALLEGRO_SAMPLE *g_Lazor;
ALLEGRO_SAMPLE *g_Screams1;
ALLEGRO_SAMPLE *g_Screams2;
ALLEGRO_SAMPLE *g_Screams3;
ALLEGRO_SAMPLE *g_Screams4;


struct LoadingException : virtual std::exception {
    LoadingException(char const* what):what_message(what){}
    char const *what() const throw(){return what_message;}
    char const *what_message;
};

struct FileLoadingException : LoadingException {
    FileLoadingException(char const* type, char const* filename): LoadingException(type), filename(filename){}
    char const *what() const throw() { return what_message; }
    char const *filename;
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
        if (display) {
            al_destroy_display(display);
        }
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

struct AllegroFontInit {
    AllegroFontInit() {
        al_init_font_addon();
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
    AllegroFontInit afont;
};

struct BitmapInit {
    BitmapInit(ALLEGRO_BITMAP **toInitialize, char const* filename):
        toInitialize(toInitialize)
    {
        Bitmap rawBitmap(al_load_bitmap(filename));
        if (!rawBitmap.get()) {
            throw FileLoadingException("Failed to load bitmap\n", filename);
        }
        int rawWidth(al_get_bitmap_width(rawBitmap.get()));
        int rawHeight(al_get_bitmap_height(rawBitmap.get()));
        
        *toInitialize = al_create_bitmap(rawWidth * 4, rawHeight * 4);
        
        if (!*toInitialize) {
            throw FileLoadingException("Failed to load bitmap\n", filename);
        }
        
        al_set_target_bitmap(*toInitialize);
        al_clear_to_color(al_map_rgba(0, 0, 0, 0));
        al_draw_scaled_bitmap(rawBitmap.get(), 0, 0, rawWidth, rawHeight, 0, 0, rawWidth*4, rawHeight*4, 0);
    }
    ~BitmapInit() {
        al_destroy_bitmap(*toInitialize);
        *toInitialize = 0;
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
            throw FileLoadingException("Failed to load sample\n", filename);
        }
    }
    ~SampleInit() {
        al_destroy_sample(*toInitialize);
        *toInitialize = 0;
    }
    private:
    ALLEGRO_SAMPLE **toInitialize;
};

struct FontInit {
    FontInit(ALLEGRO_FONT** toInitialize, ALLEGRO_BITMAP* sourceBitmap, int rangec, int const* rangev) :
        toInitialize(toInitialize)
    {
        *toInitialize = al_grab_font_from_bitmap(g_NumberSheet10, rangec, const_cast<int*>(rangev));
    }
    private:
    ALLEGRO_FONT** toInitialize;
};

struct RandInit {
    RandInit() {
        srand((unsigned)time(NULL));
    }
};

static const int font10Range[] = {43, 43, 46, 57};

struct ResourcesInit {
    ResourcesInit() :
        CaveBackground(&g_CaveBackground, "CaveBackground.png"),
        CaveEmbossedTile(&g_CaveEmbossedTile, "CaveEmbossedTile.png"),
        CaveFlameCooldownDown(&g_CaveFlameCooldownDown, "CaveFlameCooldownDown.png"),
        CaveFlameCooldownUp(&g_CaveFlameCooldownUp, "CaveFlameCooldownUp.png"),
        CaveFlamerangeDown(&g_CaveFlamerangeDown, "CaveFlamerangeDown.png"),
        CaveFlamerangeUp(&g_CaveFlamerangeUp, "CaveFlamerangeUp.png"),
        CaveReturnDown(&g_CaveReturnDown, "CaveReturnDown.png"),
        CaveReturnUp(&g_CaveReturnUp, "CaveReturnUp.png"),
        CaveReturnGrey(&g_CaveReturnGrey, "CaveReturnGrey.png"),
        CaveSizeDown(&g_CaveSizeDown, "CaveSizeDown.png"),
        CaveSizeUp(&g_CaveSizeUp, "CaveSizeUp.png"),
        CaveStaminaDown(&g_CaveStaminaDown, "CaveStaminaDown.png"),
        CaveStaminaUp(&g_CaveStaminaUp, "CaveStaminaUp.png"),
        CaveWingspanDown(&g_CaveWingspanDown, "CaveWingspanDown.png"),
        CaveWingspanUp(&g_CaveWingspanUp, "CaveWingspanUp.png"),
        DragonSmallStand(&g_DragonSmallStand, "DragonSmallStand.png"),
        LevelFG(&g_LevelFG, "levelFG.png"),
        LevelSky(&g_LevelSky, "levelSky.png"),
        GameUI(&g_GameUI, "GameUI.png"),
        MenuBackground(&g_MenuBackground, "MenuBackground.png"),
        MenuContinueDown(&g_MenuContinueDown, "MenuContinueDown.png"),
        MenuContinueGrey(&g_MenuContinueGrey, "MenuContinueGrey.png"),
        MenuContinueUp(&g_MenuContinueUp, "MenuContinueUp.png"),
        MenuExitDown(&g_MenuExitDown, "MenuExitDown.png"),
        MenuExitUp(&g_MenuExitUp, "MenuExitUp.png"),
        MenuInstructionsDown(&g_MenuInstructionsDown, "MenuInstructionsDown.png"),
        MenuInstructionsUp(&g_MenuInstructionsUp, "MenuInstructionsUp.png"),
        MenuNewGameDown(&g_MenuNewGameDown, "MenuNewGameDown.png"),
        MenuNewGameUp(&g_MenuNewGameUp, "MenuNewGameUp.png"),
        MenuTitle(&g_MenuTitle, "MenuTitle.png"),
        NumberSheet10(&g_NumberSheet10, "NumberSheet10.png"),
        Font10(&g_Font10, g_NumberSheet10, 2, font10Range),
        MenuMus(&g_MenuMus, "MenuMus.ogg"),
        DragonRoar(&g_DragonRoar, "DragonRoar.ogg"),
        FireLoop(&g_FireLoop, "FireLoop.ogg"),
        Footstep1(&g_Footstep1, "Footstep1.ogg"),
        Footstep2(&g_Footstep2, "Footstep2.ogg"),
        Footstep3(&g_Footstep3, "Footstep3.ogg"),
        Footstep4(&g_Footstep4, "Footstpe4.ogg"),
        GameMus(&g_GameMus, "GameMus.ogg"),
        Lazor(&g_Lazor, "Lazor.ogg"),
        Screams1(&g_Screams1, "Screams1.ogg"),
        Screams2(&g_Screams2, "Screams2.ogg"),
        Screams3(&g_Screams3, "Screams3.ogg"),
        Screams4(&g_Screams4, "Screams4.ogg")
    {
    }
    ~ResourcesInit() {}
    
    BitmapInit CaveBackground;
    BitmapInit CaveEmbossedTile;
    BitmapInit CaveFlameCooldownDown;
    BitmapInit CaveFlameCooldownUp;
    BitmapInit CaveFlamerangeDown;
    BitmapInit CaveFlamerangeUp;
    BitmapInit CaveReturnDown;
    BitmapInit CaveReturnGrey;
    BitmapInit CaveReturnUp;
    BitmapInit CaveSizeDown;
    BitmapInit CaveSizeUp;
    BitmapInit CaveStaminaDown;
    BitmapInit CaveStaminaUp;
    BitmapInit CaveWingspanDown;
    BitmapInit CaveWingspanUp;
    BitmapInit DragonSmallStand;
    BitmapInit LevelFG;
    BitmapInit LevelSky;
    BitmapInit GameUI;
    BitmapInit MenuBackground;
    BitmapInit MenuContinueDown;
    BitmapInit MenuContinueGrey;
    BitmapInit MenuContinueUp;
    BitmapInit MenuExitDown;
    BitmapInit MenuExitUp;
    BitmapInit MenuInstructionsDown;
    BitmapInit MenuInstructionsUp;
    BitmapInit MenuNewGameDown;
    BitmapInit MenuNewGameUp;
    BitmapInit MenuTitle;
    BitmapInit NumberSheet10;
    FontInit Font10;
    SampleInit MenuMus;
    SampleInit DragonRoar;
    SampleInit FireLoop;
    SampleInit Footstep1;
    SampleInit Footstep2;
    SampleInit Footstep3;
    SampleInit Footstep4;
    SampleInit GameMus;
    SampleInit Lazor;
    SampleInit Screams1;
    SampleInit Screams2;
    SampleInit Screams3;
    SampleInit Screams4;
};

struct Initializer {
    Initializer(){}
    AllegroInit allegroInit;
    DisplayInit displayInit;
    ResourcesInit resourcesInit;
    RandInit randInit;
};

void mainLoop(std::auto_ptr<Scene> screen);

int main(int argc, char **argv){
    try {
        Initializer init;
        
        std::auto_ptr<MainMenu> mainMenu(new MainMenu());
        mainMenu->init();
        mainLoop(std::auto_ptr<Scene>(mainMenu.release()));
        
        return 0;
    }
    catch (FileLoadingException const& e) {
        std::cerr << "File Loading Failed:\n";
        std::cerr << e.what();
        std::cerr << e.filename << '\n';
        return -1;
    }
    catch (std::exception const& e) {
        std::cerr << "Failed:\n";
        std::cerr << e.what();
        return -1;
    }
    catch (...) {
        std::cerr << "Failed for unknown reasons.\n";
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

