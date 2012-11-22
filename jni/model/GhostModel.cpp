#include <jni.h>  
#include <string.h>
#include <cstdlib> 
#include <android/log.h> 
#include <math.h> 

#include "model/GhostModel.h"
#include "model/PacManModel.h"
#include "data/IntPosition.h"
#include "data/FloatPosition.h"
#include "data/MotionDirection.h"
#include "data/TileType.h"
  
#define DEBUG_TAG "NDK_GhostModel"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,DEBUG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,DEBUG_TAG,__VA_ARGS__)
  
namespace game{
    static long waveLengthMap[5][8] = {
        {7000, 20000, 7000, 20000, 5000, 20000, 5000, -1},
        {7000, 20000, 7000, 20000, 5000, 1033000, 1/60, -1},
        {7000, 20000, 7000, 20000, 5000, 1033000, 1/60, -1},
        {7000, 20000, 7000, 20000, 5000, 1033000, 1/60, -1},
        {5000, 20000, 5000, 20000, 5000, 1037000, 1/60, -1}
    };

    GhostModel::GhostModel ( 
            PathArray &path,
            int permissionLevel,
            int defaultHomePermission,
            PacManModel* pacManModel,
            IntPosition* initialPosition,
            IntPosition* scatterPosition,
            int tabletsToLeaveHome
        ) : PersonModel ( path, permissionLevel ) {
        
        DEFAULT_ANIMATION_DURATION = 200.0f;
        animationDuration = DEFAULT_ANIMATION_DURATION;
        
        motionDirectionOfReturn = MotionDirection::NONE;
        availableExitsNumber = 0;
        
        DEFAULT_OFFSET = new FloatPosition(0.0f, 0.0f);
        HOME_OFFSET = new FloatPosition(0.5f, 0.5f);
        homeOffset = DEFAULT_OFFSET;
        
        currentLevel = 0;

        FRIGHT_DURATION = 10000L;
        
        tabletsTimeToLeftHome = 10000;
        
        this->defaultHomePermission = defaultHomePermission;
		this->homePermission = defaultHomePermission;
		this->pacManModel = pacManModel;
        
        INITIAL_POSITION = initialPosition;
		SCATTER_POSITION = scatterPosition;
		LEAVE_HOME_POSITION = new IntPosition(14,11);
		GET_BODY_POSITION = new IntPosition(14,14);
        
        this->tabletsToLeftHome = tabletsToLeftHome;
        
        reset(0L);
    }
    
    void GhostModel::reset(long time) {
        currentPosition = INITIAL_POSITION;
		nextPosition = currentPosition;
		isTrapped = (path[INITIAL_POSITION->getY()][INITIAL_POSITION->getX()] & TileType::GHOST_TRAP) == TileType::GHOST_TRAP;
		atHome = (path[INITIAL_POSITION->getY()][INITIAL_POSITION->getX()] & TileType::GHOST_HOME) == TileType::GHOST_HOME;
		if (atHome)
			homeOffset = HOME_OFFSET;
		else
			homeOffset = new FloatPosition(HOME_OFFSET->getX(), 0.0f);
		isChasing = false;
	    isFrightened = false;
	    isReturning = false;
	    
        needChangeDirection = false;
		lastChaseModeChangeAt = time;
		lastFrightenModeChangeAt = time;
        lastFrightenBegunAt = 0;
        currentModeWave = 0;
        
        lastTabletEatenAt = time;
    
        currentMotionDirection = MotionDirection::NONE;
        desiredMotionDirection = MotionDirection::NONE;
        
        currentPosition = INITIAL_POSITION;
        nextPosition = currentPosition;
        
        lastTick = time;
        moveAnimationPhase = 0.0f;
        lastTabletEatenAt = time;
    }
    
    void GhostModel::forceDirectionChange() {
        if (isFrightened || isReturning)
			return;
		needChangeDirection = true;
    }
    
    void GhostModel::processTime (long time) {
		long currentWaveDuration = time - lastChaseModeChangeAt;
		if ((waveLengthMap[currentLevel][currentModeWave] > 0) && (currentWaveDuration > waveLengthMap[currentLevel][currentModeWave])) {
			lastChaseModeChangeAt = time;
			isChasing = !isChasing;
			currentModeWave++;
			forceDirectionChange();
		}
		/**
		 * TODO: add frighten length change with level
		 */
		if (isFrightened) {
			long currentFrightDuration = time - lastFrightenModeChangeAt;
			if (currentFrightDuration > FRIGHT_DURATION) {
				lastFrightenModeChangeAt = time;
				isFrightened = false;
				forceDirectionChange();
			}
		}
		
		if (isTrapped && ((time - tabletsTimeToLeftHome) > lastTabletEatenAt)) {
			isTrapped = false;
		}
	}
    
    FloatPosition* GhostModel::getHomeDisp() {
        return homeOffset;
    }
    
    char GhostModel::getIsChasing() {
        return isChasing;
    }
    
    char GhostModel::getIsFrightened() {
        return isFrightened;
    }
    
    char GhostModel::getIsReturning() {
        return isReturning;
    }
    
    long GhostModel::timeInFrightenStateLeft(long time) {
        return (FRIGHT_DURATION - time + lastFrightenModeChangeAt);
    }
    
    /*void GhostModel::setGhostEatenHandler(IGhostEatenHandler* value) {
        ghostEatenHandler = value;
    }*/
    
    IntPosition* GhostModel::calculateTargetPosition() {
        IntPosition* pacManPosition = pacManModel->getCurrentPosition();
		IntPosition* pacManPositionDisplacement = pacManModel->getPositionDisplacement();
		return new IntPosition (pacManPosition->getX() + pacManPositionDisplacement->getX(), pacManPosition->getY() + pacManPositionDisplacement->getY());
    }
    
    int GhostModel::estimateDistanceToTargetPoint(IntPosition* disp) {
        int dX = targetPosition->getX() - currentPosition->getX() - disp->getX();
		int dY = targetPosition->getY() - currentPosition->getY() - disp->getY();
		
		return (dX * dX + dY * dY); 
    }
    
    void GhostModel::frighten(long time) {
        if (isReturning)
			return;
		isFrightened = true;
		lastFrightenModeChangeAt = time;
    }
    
    void GhostModel::eaten (int score) {
		isFrightened = false;
		isReturning = true;
		homePermission = homePermission | TileType::GHOST_HOME;
		
		/*if (ghostEatenHandler != null)
			ghostEatenHandler->handleGhostEaten(currentPosition, score);*/
	}
    
    void GhostModel::step(long time) {
		processTime (time);
		
        long delta = time - lastTick;
        moveAnimationPhase = (float)delta / (float)animationDuration;
        if (delta > animationDuration) {
        	moveAnimationPhase = 0.0f;
        	lastTick = time;
        	animationDuration = DEFAULT_ANIMATION_DURATION;
        	currentPosition = nextPosition;
        	
        	if (isReturning) {
        		if (currentPosition->equals(GET_BODY_POSITION)) {
        			isReturning = false;
        		}
        		homePermission = homePermission | TileType::GHOST_HOME;
        		atHome = (path[currentPosition->getY()][currentPosition->getX()] & homePermission) != 0;
        	} else {
        		homePermission = (path[currentPosition->getY()][currentPosition->getX()] & homePermission);
            	atHome = (homePermission & TileType::GHOST_HOME) == TileType::GHOST_HOME;
        	}
        	
        	if ((currentPosition->getX() == 1) && (currentMotionDirection == MotionDirection::LEFT)) {
        		currentPosition = new IntPosition(FieldSize::WIDTH - 2, currentPosition->getY());
        	} else if ((currentPosition->getX() == (FieldSize::WIDTH - 2)) && (currentMotionDirection == MotionDirection::RIGHT)){
        		currentPosition = new IntPosition(1, currentPosition->getY());
        	}
        	
        	availableExitsNumber = 0;
        	/**
        	 * TODO: fix forced turn around
        	 */
        	if (false/*needChangeDirection*/) {
        		needChangeDirection = false;
        		currentMotionDirection = currentMotionDirection->getReturnDirection();
        	} else {
        		currentMotionDirection = MotionDirection::NONE;
	        	for (int i = 0; i < 4; i++) {
	        		int currentX = currentPosition->getX() + MotionDirection::POSSIBLE_MOTION_DIRECTIONS[i]->getPositionDisplacement()->getX();
	        		int currentY = currentPosition->getY() + MotionDirection::POSSIBLE_MOTION_DIRECTIONS[i]->getPositionDisplacement()->getY();
	        		int currentPermission = path[currentY][currentX];
	        		if (
	        				(atHome || (MotionDirection::POSSIBLE_MOTION_DIRECTIONS[i] != motionDirectionOfReturn))
	        				&& (
	        					(isTrapped && ((currentPermission & TileType::GHOST_TRAP) != 0))
	        					|| (!isTrapped && ((currentPermission & permissionLevel) != 0))
	        					|| (!isTrapped && ((currentPermission & homePermission) != 0))
	        				)
	        			) {
	        			availableExits[availableExitsNumber++] = MotionDirection::POSSIBLE_MOTION_DIRECTIONS[i];
	        		}
	        	}
        	
	        	if (availableExitsNumber == 0) {
        			currentMotionDirection = motionDirectionOfReturn;
	        	} else if (availableExitsNumber == 1) {
	        		for (int ae = 0; ae < availableExitsNumber; ae++) {
	        			currentMotionDirection = availableExits[ae];
	        		}
	        	} else {
	        		int minDistance = 10000;
	        		if (isReturning) {
	        			targetPosition = GET_BODY_POSITION;
        				for (int ae = 0; ae < availableExitsNumber; ae++) {
    	        			int newDistance = estimateDistanceToTargetPoint(availableExits[ae]->getPositionDisplacement());
    	        		    if (minDistance > newDistance) {
    	        		    	minDistance = newDistance;
    	        		    	currentMotionDirection = availableExits[ae];
    	        		    }
    	        		}
	        		} else if (atHome) {
	        			targetPosition = LEAVE_HOME_POSITION;
		        		for (int ae = 0; ae < availableExitsNumber; ae++) {
    	        			int newDistance = estimateDistanceToTargetPoint(availableExits[ae]->getPositionDisplacement());
    	        		    if (minDistance > newDistance) {
    	        		    	minDistance = newDistance;
    	        		    	currentMotionDirection = availableExits[ae];
    	        		    }
    	        		}
	        		} else if (isFrightened) {
        				long exitNumber = floor(rand() / RAND_MAX * availableExitsNumber + 0.5);
                        currentMotionDirection = availableExits[exitNumber];
	        		} else if (isChasing) {
	        			targetPosition = calculateTargetPosition();
        				for (int ae = 0; ae < availableExitsNumber; ae++) {
    	        			int newDistance = estimateDistanceToTargetPoint(availableExits[ae]->getPositionDisplacement());
    	        		    if (minDistance > newDistance) {
    	        		    	minDistance = newDistance;
    	        		    	currentMotionDirection = availableExits[ae];
    	        		    }
    	        		}
	        		} else {
	        			targetPosition = SCATTER_POSITION;
    	        		for (int ae = 0; ae < availableExitsNumber; ae++) {
    	        			int newDistance = estimateDistanceToTargetPoint(availableExits[ae]->getPositionDisplacement());
    	        		    if (minDistance > newDistance) {
    	        		    	minDistance = newDistance;
    	        		    	currentMotionDirection = availableExits[ae];
    	        		    }
    	        		}
	        		}
	        	}
        	}
        	
        	motionDirectionOfReturn = currentMotionDirection->getReturnDirection();
        	
        	nextPosition = new IntPosition(currentPosition->getX() + currentMotionDirection->getPositionDisplacement()->getX(), currentPosition->getY() + currentMotionDirection->getPositionDisplacement()->getY());
        	if ((homeOffset->getX() != 0.0f) && ((path[nextPosition->getY()][nextPosition->getX()] & TileType::GHOST_HOME) == 0)) {
        		if (currentMotionDirection->getPositionDisplacement()->getX() > 0) {
	        		animationDuration = animationDuration * (1 + homeOffset->getX());
	        		lastTick = lastTick - (long)(animationDuration * homeOffset->getX());
	        		moveAnimationPhase = homeOffset->getX();
	        		homeOffset = new FloatPosition(0.0f, homeOffset->getY());
        		} else if (currentMotionDirection->getPositionDisplacement()->getX() < 0) {
        			animationDuration = animationDuration * (1 + homeOffset->getX());
	        		lastTick = lastTick + (long)(animationDuration * homeOffset->getX());
	        		moveAnimationPhase = -homeOffset->getX();
	        		homeOffset = new FloatPosition(0.0f, homeOffset->getY());
        		}
        		
        	} else if ((homeOffset->getX() == 0.0f) && (currentMotionDirection->getPositionDisplacement()->getX() != 0) && ((path[nextPosition->getY()][nextPosition->getX()] & TileType::GHOST_HOME) == TileType::GHOST_HOME)) {
        		if (currentMotionDirection->getPositionDisplacement()->getX() > 0) {
//	        		Log.d("outX", "11 " + animationDuration + " " + lastTick + " " + moveAnimationPhase + " " + homeOffset->getX());
	        		homeOffset = new FloatPosition(HOME_OFFSET->getX(), homeOffset->getY());
	        		animationDuration = animationDuration * (1 + homeOffset->getX());
	        		lastTick =  lastTick - (long)(animationDuration * homeOffset->getX());
	        		moveAnimationPhase = homeOffset->getX();
//	        		Log.d("outX", "12 " + animationDuration + " " + lastTick + " " + moveAnimationPhase + " " + homeOffset->getX());
        		} else if (currentMotionDirection->getPositionDisplacement()->getX() < 0) {
//        			Log.d("outX", "21 " + animationDuration + " " + lastTick + " " + moveAnimationPhase + " " + homeOffset->getX());
	        		homeOffset = new FloatPosition(HOME_OFFSET->getX(), homeOffset->getY());
	        		animationDuration = animationDuration * (1 + homeOffset->getX());
	        		lastTick =  lastTick - (long)(animationDuration * homeOffset->getX());
	        		moveAnimationPhase = homeOffset->getX();
//	        		Log.d("outX", "22 " + animationDuration + " " + lastTick + " " + moveAnimationPhase + " " + homeOffset->getX());
        		}
        	}
        	
        	if ((homeOffset->getY() != 0.0f) && (currentMotionDirection->getPositionDisplacement()->getY() != 0) && ((path[nextPosition->getY()][nextPosition->getX()] & TileType::GHOST_HOME) == 0)) {
        		if (currentMotionDirection->getPositionDisplacement()->getY() < 0) {
	        		animationDuration = animationDuration * (1 + homeOffset->getY());
	        		lastTick =  lastTick + (long)(animationDuration * homeOffset->getY());
	        		moveAnimationPhase = -homeOffset->getY();
	        		homeOffset = new FloatPosition(homeOffset->getX(), 0.0f);
        		} else if (currentMotionDirection->getPositionDisplacement()->getY() > 0) {
        			animationDuration = animationDuration * (1 - homeOffset->getY());
	        		lastTick =  lastTick - (long)(animationDuration * homeOffset->getY());
	        		moveAnimationPhase = homeOffset->getY();
	        		homeOffset = new FloatPosition(homeOffset->getX(), 0.0f);
        		}
        	} else if ((homeOffset->getY() == 0.0f) && (currentMotionDirection->getPositionDisplacement()->getY() != 0) && ((path[nextPosition->getY()][nextPosition->getX()] & TileType::GHOST_HOME) == TileType::GHOST_HOME)) {
        		if (currentMotionDirection->getPositionDisplacement()->getY() < 0) {
	        		homeOffset = new FloatPosition(homeOffset->getX(), HOME_OFFSET->getY());
	        		animationDuration = animationDuration * (1 + homeOffset->getY());
	        		lastTick =  lastTick - (long)(animationDuration * homeOffset->getY());
	        		moveAnimationPhase = homeOffset->getY();
        		} else if (currentMotionDirection->getPositionDisplacement()->getY() > 0) {
        			/** 
        			 * this case is not properly tested as it is not possible with the current map configuration
        			 */
        			homeOffset = new FloatPosition(homeOffset->getX(), HOME_OFFSET->getY());
	        		animationDuration = animationDuration * (1 - homeOffset->getY());
	        		lastTick =  lastTick + (long)(animationDuration * homeOffset->getY());
	        		moveAnimationPhase = -homeOffset->getY();
        		}
        	}
    	}
    }
    
    IntPosition* GhostModel::getCurrentPosition () {
		return currentPosition;
	}
	
	IntPosition* GhostModel::getPositionDisplacement () {
		return currentMotionDirection->getPositionDisplacement();
	}
	
	float GhostModel::getMoveAnimationPhase () {
		return moveAnimationPhase;
	}

	float GhostModel::getAngle () {
		return currentMotionDirection->getAngle();
	}
    
    void GhostModel::tabletEaten (long time, int tabletsEaten) {
		lastTabletEatenAt = time;
		if (isTrapped && (tabletsEaten > tabletsToLeftHome))
			isTrapped = false;
	}
}