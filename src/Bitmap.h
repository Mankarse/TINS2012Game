#ifndef TINSGame2012_Bitmap_h
#define TINSGame2012_Bitmap_h
#include <allegro5/allegro.h>
struct Bitmap {
    Bitmap(ALLEGRO_BITMAP *bitmap=0): bitmap(bitmap){}
    
    ALLEGRO_BITMAP* get() {
        return bitmap;
    }
    
    ALLEGRO_BITMAP* release() {
        ALLEGRO_BITMAP *retV = bitmap;
        bitmap = 0;
        return retV;
    }
    
    void reset(ALLEGRO_BITMAP* const bitmap=0) {
        al_destroy_bitmap(this->bitmap);
        this->bitmap = bitmap;
    }
    
    ~Bitmap(){
        al_destroy_bitmap(bitmap);
    }
private:
    Bitmap(Bitmap const&);
    Bitmap& operator=(Bitmap const&);
    ALLEGRO_BITMAP *bitmap;
};
#endif
