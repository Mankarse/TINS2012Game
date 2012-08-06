#ifndef TINSGame2012_Globals_h
#define TINSGame2012_Globals_h
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <map>
#include <string>
extern ALLEGRO_DISPLAY *g_display;

extern std::map<std::string, ALLEGRO_BITMAP*> g_Bitmaps;

extern ALLEGRO_FONT *g_Font10;

extern ALLEGRO_SAMPLE *g_MenuMus;
extern ALLEGRO_SAMPLE *g_DragonRoar;
extern ALLEGRO_SAMPLE *g_FireLoop;
extern ALLEGRO_SAMPLE *g_Footstep1;
extern ALLEGRO_SAMPLE *g_Footstep2;
extern ALLEGRO_SAMPLE *g_Footstep3;
extern ALLEGRO_SAMPLE *g_Footstep4;
extern ALLEGRO_SAMPLE *g_GameMus;
extern ALLEGRO_SAMPLE *g_Lazor;
extern ALLEGRO_SAMPLE *g_Screams1;
extern ALLEGRO_SAMPLE *g_Screams2;
extern ALLEGRO_SAMPLE *g_Screams3;
extern ALLEGRO_SAMPLE *g_Screams4;


#endif
