#ifndef TINSGame2012__Engine
#define TINSGame2012__Engine
#include <memory>

struct Continuation {};

struct NormalContinuation:Continuation{

};

struct CaveContinuation:Continuation{
    //Cave/Save Data (i.e. score, score delta, dragon stats, spawner stats)
};

struct LoseContinuation:Continuation{
    //Final Score
};

/*struct PauseContinuation:Continuation{
    Bitmap currentView;
};
*/

struct ExitContinuation:Continuation{
};

class Engine {
    public:
    std::auto_ptr<Continuation> logicStep();
    
    void render();
    //
    //
    //
    //
};

#endif /* defined(TINSGame2012__Engine) */
