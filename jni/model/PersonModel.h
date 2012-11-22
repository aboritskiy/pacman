#ifndef PERSON_MODEL
#define PERSON_MODEL

#include "data/FieldSize.h"

namespace game{
    class IntPosition;
    class MotionDirection;
 
    class PersonModel {
        protected:
        PathArray &path;
        int permissionLevel;
        
        IntPosition* currentPosition;
        IntPosition* nextPosition;
        IntPosition* positionDisplacement;
        
        MotionDirection* currentMotionDirection;
        MotionDirection* desiredMotionDirection;
    
        public:
        PersonModel( PathArray &aPath, int aPermissionLevel );
        IntPosition* getCurrentPosition ();
        IntPosition* getNextPosition ();
        
        void setMotionDirection ( MotionDirection* md);
        void step(long time);
        void reset(long time);
    };
}

#endif 