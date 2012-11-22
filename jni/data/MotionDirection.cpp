#include <android/log.h>  

#include "MotionDirection.h"

#include "IntPosition.h"
  
#define DEBUG_TAG "NDK_MotionDirection"
  
namespace game{
    MotionDirection* MotionDirection::LEFT  = new MotionDirection ( new IntPosition(-1, 0), 180.0f, 1 );
    MotionDirection* MotionDirection::RIGHT = new MotionDirection ( new IntPosition( 1, 0),   0.0f, 0 );
    MotionDirection* MotionDirection::UP    = new MotionDirection ( new IntPosition( 0,-1),  90.0f, 3 );
    MotionDirection* MotionDirection::DOWN  = new MotionDirection ( new IntPosition( 0, 1), 270.0f, 2 );
    MotionDirection* MotionDirection::NONE  = new MotionDirection ( new IntPosition( 0, 0),   0.0f, 4 );
    
    MotionDirection* MotionDirection::POSSIBLE_MOTION_DIRECTIONS[5] = {
        MotionDirection::LEFT,
        MotionDirection::RIGHT,
        MotionDirection::UP,
        MotionDirection::DOWN,
        MotionDirection::NONE
    };

    MotionDirection::MotionDirection( IntPosition* positionDisplacement, float angle, int returnDirectionIndex ) {
        this->positionDisplacement = positionDisplacement;
        this->angle = angle;
        this->returnDirectionIndex = returnDirectionIndex;
    }
        
    IntPosition* MotionDirection::getPositionDisplacement() {
        return positionDisplacement;
    }
    
    float MotionDirection::getAngle() {
        return angle;
    }
    
    MotionDirection* MotionDirection::getReturnDirection() {
        return MotionDirection::POSSIBLE_MOTION_DIRECTIONS[returnDirectionIndex];
    }
}