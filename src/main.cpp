#include "Globals.h"

#include <iostream>
#include <fstream>
#include <exception>
#include <allegro5/allegro.h>
#include <memory>
#include <list>
#include <cstdlib>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include "MainMenu.h"
#include "Cave.h"
#include "Bitmap.h"
#include "EventQueue.h"
#include "Timer.h"
#include "Filesystem.h"

StrictMap<std::string, ALLEGRO_BITMAP*> g_Bitmaps;
StrictMap<std::string, ALLEGRO_SAMPLE*> g_Samples;

ALLEGRO_DISPLAY *g_display;

ALLEGRO_FONT *g_Font10;


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

ALLEGRO_BITMAP* loadAndScaleBitmap(char const* filename) {
        Bitmap rawBitmap(al_load_bitmap(filename));
        if (!rawBitmap.get()) {
            throw FileLoadingException("Failed to load bitmap\n", filename);
        }
        int rawWidth(al_get_bitmap_width(rawBitmap.get()));
        int rawHeight(al_get_bitmap_height(rawBitmap.get()));
        
        ALLEGRO_BITMAP* scaledBitmap = al_create_bitmap(rawWidth * 4, rawHeight * 4);
        
        if (!scaledBitmap) {
            throw FileLoadingException("Failed to load bitmap\n", filename);
        }
        
        al_set_target_bitmap(scaledBitmap);
        al_clear_to_color(al_map_rgba(0, 0, 0, 0));
        al_draw_scaled_bitmap(rawBitmap.get(), 0, 0, rawWidth, rawHeight, 0, 0, rawWidth*4, rawHeight*4, 0);
        return scaledBitmap;
}

struct BitmapInit {
    BitmapInit(ALLEGRO_BITMAP **toInitialize, char const* filename):
        toInitialize(toInitialize)
    {
        *toInitialize = loadAndScaleBitmap(filename);
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
        *toInitialize = al_grab_font_from_bitmap(g_Bitmaps["NumberSheet10"], rangec, const_cast<int*>(rangev));
    }
    ~FontInit() {
        al_destroy_font(*toInitialize);
    }
    private:
    ALLEGRO_FONT** toInitialize;
};

struct RandInit {
    RandInit() {
        srand((unsigned)time(NULL));
    }
};

struct BitmapManager {
    BitmapManager() : bitmap() {}
    ALLEGRO_BITMAP* init(ALLEGRO_BITMAP* const bitmap) {
        return this->bitmap = bitmap;
    }
    BitmapManager(BitmapManager const& o): bitmap() {
        assert(!o.bitmap);
    }
    BitmapManager& operator=(BitmapManager const& o) {
        assert(!o.bitmap);
        assert(!bitmap);
        return *this;
    }
    ~BitmapManager() {
        al_destroy_bitmap(bitmap);
    }
    ALLEGRO_BITMAP *bitmap;
};

ALLEGRO_SAMPLE* loadSample(char const* filename){
    ALLEGRO_SAMPLE* sample = al_load_sample(filename);
    if (!sample) {
        throw FileLoadingException("Failed to load sample\n", filename);
    }
    return sample;
}

struct SampleManager {
    SampleManager() : sample() {}
    ALLEGRO_SAMPLE* init(char const* filename) {
        return sample = loadSample(filename);
    }
    SampleManager(SampleManager const& o): sample() {
        assert(!o.sample);
    }
    SampleManager& operator=(SampleManager const& o) {
        assert(!o.sample);
        assert(!sample);
        return *this;
    }
    ~SampleManager() {
        if (sample) {
            al_destroy_sample(sample);
        }
    }
    ALLEGRO_SAMPLE *sample;
};

bool endsWith(std::string const& haystack, std::string const& needle) {
    return haystack.size()>=needle.size() && haystack.substr(haystack.size()-needle.size())==needle.c_str();
}

struct SmartInit {
    SmartInit() {
        FilesystemEntry resourceDirectory(al_create_fs_entry("."));
        if (!resourceDirectory.get()) {
            throw LoadingException("Could not init resources directory");
        }
        DirectoryManager directory(resourceDirectory.get());
        if (!directory.is_open()) { throw LoadingException("Could not open resources directory"); }
        
        FilesystemEntry resourceFile;
        while (resourceFile.reset(al_read_directory(resourceDirectory.get())), resourceFile.get()) {
            std::string filename(al_get_fs_entry_name(resourceFile.get()));
            
            if (endsWith(filename,std::string(".png"))) {
                filename = filename.substr(2);
                bitmapManagers.push_back(BitmapManager());
                g_Bitmaps.insert(std::make_pair(filename.substr(0,filename.size()-4), bitmapManagers.back().init(loadAndScaleBitmap(filename.c_str()))));
            }
            if (endsWith(filename, std::string(".ogg"))) {
                filename = filename.substr(2);
                sampleManagers.push_back(SampleManager());
                g_Samples.insert(std::make_pair(filename.substr(0,filename.size()-4), sampleManagers.back().init(filename.c_str())));
            }
        }
    }
    //Using std::list because insertion does not invaliate iterators.
    std::list<BitmapManager> bitmapManagers;
    std::list<SampleManager> sampleManagers;
};

static ALLEGRO_BITMAP* loadSubBitmap(ALLEGRO_BITMAP* parent, int x, int y, int w, int h) {
    Bitmap bitmap(al_create_sub_bitmap(parent, x, y, w, h));
    if (!bitmap.get()) {
        throw LoadingException("Failed at splitting bitmap.");
    }
    return bitmap.release();
}

struct SpritesInit {
    SpritesInit() {
        std::ifstream spriteManifest("SpriteSheet.txt");
        while (spriteManifest) {
            std::string name;
            std::string eq;
            int x;
            int y;
            int width;
            int height;
            spriteManifest >> name >> eq >> x >> y >> width >> height;
            bitmapManagers.push_back(BitmapManager());
            g_Bitmaps.insert(std::make_pair(name, bitmapManagers.back().init(loadSubBitmap(g_Bitmaps["SpriteSheet"], x*4, y*4, width*4, height*4))));
        }
    }
    std::list<BitmapManager> bitmapManagers;
};

static const int font10Range[] = {43, 43, 46, 57};

struct ResourcesInit {
    ResourcesInit() :
        init(),
        Font10(&g_Font10, g_Bitmaps["NumberSheet10"], 2, font10Range),
        sprites()
    {
    }
    
    SmartInit init;
    SpritesInit sprites;
    FontInit Font10;
};

struct Initializer {
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
    Timer timer(al_create_timer(1.0 / fps));
    EventQueue event_queue(al_create_event_queue());
    al_register_event_source(event_queue.get(), al_get_display_event_source(g_display));
    al_register_event_source(event_queue.get(), al_get_timer_event_source(timer.get()));
    al_register_event_source(event_queue.get(), al_get_keyboard_event_source());
    al_register_event_source(event_queue.get(), al_get_mouse_event_source());

    al_start_timer(timer.get());

    //LagLimiter lag;
    InputState input(timer.get());
    while (scene.get()) {
        //readInput reads until the timer triggers
        //so it is what regulates game speed.
        input.updateState(event_queue.get());
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

