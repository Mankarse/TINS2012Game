//
//  GameObject.cpp
//  TINSGame2012
//
//  Created by Evan Wallace on 4/08/12.
//
//

#include "GameObject.h"

ALLEGRO_TRANSFORM GameObject::getWorldTransform()
{
    ALLEGRO_TRANSFORM retV;
    al_identity_transform(&retV);
    al_compose_transform(&retV, &myTrans);
    if(getParent() != 0)
    {
        ALLEGRO_TRANSFORM parentTransform(getParent()->getWorldTransform());
        al_compose_transform(&retV, &parentTransform);
    }
    return retV;
}

GameObject *GameObject::getParent()
{
    return parent;
}