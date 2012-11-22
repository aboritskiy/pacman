#ifndef MOTION_DIRECTION
#define MOTION_DIRECTION

namespace game{
    class IntPosition;
    class MotionDirection;
 
    class MotionDirection {
        protected:
        IntPosition* positionDisplacement;
        float angle;
        int returnDirectionIndex;

        public:
        MotionDirection( IntPosition* positionDisplacement, float angle, int returnDirectionIndex );
        
        IntPosition* getPositionDisplacement();
        float getAngle();
        MotionDirection* getReturnDirection();
        
        static MotionDirection* NONE;
        static MotionDirection* LEFT;
        static MotionDirection* RIGHT;
        static MotionDirection* UP;
        static MotionDirection* DOWN;
        
        static MotionDirection* POSSIBLE_MOTION_DIRECTIONS[5];
    };
}

#endif 