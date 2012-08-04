//
//  GameObject.h
//  TINSGame2012
//
//  Created by Evan Wallace on 4/08/12.
//
//

#ifndef TINSGame2012__GameObject
#define TINSGame2012__GameObject
#include <allegro5/allegro.h>
class GameObject {
    private:
    ALLEGRO_TRANSFORM myTrans;
    GameObject *parent;
    ALLEGRO_BITMAP *renderer;
    
    public:
    ALLEGRO_TRANSFORM getWorldTransform();
    GameObject *getParent();
};

#endif /* defined(__TINSGame2012__GameObject__) */
