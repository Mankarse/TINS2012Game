#ifndef TINSGame2012_Globals_h
#define TINSGame2012_Globals_h
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <map>
#include <string>
extern ALLEGRO_DISPLAY *g_display;

extern std::map<std::string, ALLEGRO_BITMAP*> g_Bitmaps;
extern std::map<std::string, ALLEGRO_SAMPLE*> g_Samples;

extern ALLEGRO_FONT *g_Font10;

#endif
