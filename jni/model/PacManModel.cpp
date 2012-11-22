#include <jni.h>  
#include <string.h>  
#include <android/log.h>  

#include "PacManModel.h"
#include "data/IntPosition.h"
#include "data/MotionDirection.h"
  
#define DEBUG_TAG "NDK_PacManModel"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,DEBUG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,DEBUG_TAG,__VA_ARGS__)
  
namespace game{

    PacManModel::PacManModel ( PathArray &path, int permissionLevel )
        : PersonModel ( path, permissionLevel ) {
        
        INITIAL_POSITION = new IntPosition (14,23);
        
        reset(0L);
    }
    
    void PacManModel::reset(long time) {
        currentMotionDirection = MotionDirection::NONE;
        desiredMotionDirection = MotionDirection::NONE;
        
        currentPosition = INITIAL_POSITION;
        nextPosition = currentPosition;
        
        moveAnimationPhase = 0.0f;
        angle = 0.0f;
    }
    
    void PacManModel::step(long time) {
        time = time % 200L;
        float newPhase = (float)time / 200.0f;
        
        if (newPhase < moveAnimationPhase) {
        	currentPosition = nextPosition;
            
        	if ((currentPosition->getX() == 1) && (currentMotionDirection == MotionDirection::LEFT)) {
        		currentPosition = new IntPosition(FieldSize::WIDTH - 2, currentPosition->getY());
        	} else if ((currentPosition->getX() == (FieldSize::WIDTH - 2)) && (currentMotionDirection == MotionDirection::RIGHT)){
        		currentPosition = new IntPosition(1, currentPosition->getY());
        	}
        	
        	int desiredX = currentPosition->getX() + desiredMotionDirection->getPositionDisplacement()->getX();
        	int desiredY = currentPosition->getY() + desiredMotionDirection->getPositionDisplacement()->getY();
        	int newX = currentPosition->getX() + currentMotionDirection->getPositionDisplacement()->getX();
        	int newY = currentPosition->getY() + currentMotionDirection->getPositionDisplacement()->getY();
            
        	if ((path[desiredY][desiredX] & permissionLevel) != 0 ) {
        		nextPosition = new IntPosition(desiredX, desiredY);
        		angle = desiredMotionDirection->getAngle();
        		currentMotionDirection = desiredMotionDirection;
        	} else if ((path[newY][newX] & permissionLevel) != 0 )  {
        		nextPosition = new IntPosition(newX, newY);
        	} else {
        		currentMotionDirection = MotionDirection::NONE;
        	}
        }
        
        moveAnimationPhase = newPhase;
    }
    
    IntPosition* PacManModel::getCurrentPosition () {
		return currentPosition;
	}
	
	IntPosition* PacManModel::getPositionDisplacement () {
		return currentMotionDirection->getPositionDisplacement();
	}
	
	float PacManModel::getMoveAnimationPhase () {
		return moveAnimationPhase;
	}

	float PacManModel::getAngle () {
		return angle;
	}
}