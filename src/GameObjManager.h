
#ifndef TINSGame2012__GameObjManager
#define TINSGame2012__GameObjManager
#include <vector>
#include "GameObject.h"
class GameObjManager
{
    public:
    std::vector<GameObject*> existingObjects;
    
    void addObject(GameObject* newObject);
};

#endif /* defined(TINSGame2012__GameObjManager) */
