#ifndef PACMAN_MODEL
#define PACMAN_MODEL

#include "PersonModel.h"
#include "data/FieldSize.h"

namespace game{
    class PersonModel;
    class IntPosition;
 
    class PacManModel : public PersonModel{
        private:
        IntPosition* INITIAL_POSITION;
        float moveAnimationPhase;
        float angle;
    
        public:
        PacManModel( PathArray &aPath, int aPermissionLevel );
        void reset(long time);
        void step(long time);
        IntPosition* getCurrentPosition ();
        IntPosition* getPositionDisplacement();
        float getMoveAnimationPhase();
        float getAngle();
        
    };
}

#endif 