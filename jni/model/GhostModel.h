#ifndef GHOST_MODEL
#define GHOST_MODEL

#include "PersonModel.h"
#include "data/FieldSize.h"

namespace game{
    class PersonModel;
    class IntPosition;
    class FloatPosition;
    class PacManModel;
    class TileType;
    class IGhostEatenHandler;
 
    class GhostModel : public PersonModel{
        protected:
        IntPosition* INITIAL_POSITION;
        IntPosition* SCATTER_POSITION;
        IntPosition* LEAVE_HOME_POSITION;
        IntPosition* GET_BODY_POSITION;
        
        float DEFAULT_ANIMATION_DURATION;
        float animationDuration;
        long lastTick;
        float moveAnimationPhase;
        
        int defaultHomePermission;
        int homePermission;
        
        PacManModel* pacManModel;
        MotionDirection* motionDirectionOfReturn;
        MotionDirection* availableExits[4];
        int availableExitsNumber;
        
        IntPosition* targetPosition;
        
        FloatPosition* DEFAULT_OFFSET;
        FloatPosition* HOME_OFFSET;
        FloatPosition* homeOffset;
        
        char isTrapped;
        char atHome;
        char isChasing;
        char isFrightened;
        char isReturning;
        
        char needChangeDirection;
        long lastChaseModeChangeAt;
        int currentModeWave;
        int currentLevel;
        
        long lastFrightenModeChangeAt;
        
        long lastFrightenBegunAt;
        long FRIGHT_DURATION; 
        
        IGhostEatenHandler* ghostEatenHandler;
        
        int tabletsToLeaveHome;
        int tabletsTimeToLeaveHome;
        long lastTabletEatenAt;
        
        void forceDirectionChange ();
        void processTime (long time);
        IntPosition* calculateTargetPosition();
        int estimateDistanceToTargetPoint(IntPosition* disp);
    
        public:
        GhostModel ( 
            PathArray &path,
            int permissionLevel,
            int defaultHomePermission,
            PacManModel* pacManModel,
            IntPosition* initialPosition,
            IntPosition* scatterPosition,
            int tabletsToLeaveHome
        );
        void reset(long time);
        void step(long time);
        IntPosition* getCurrentPosition ();
        IntPosition* getPositionDisplacement();
        float getMoveAnimationPhase();
        float getAngle();
        
        FloatPosition* getHomeDisp();
        char getIsChasing();
        char getIsFrightened();
        char getIsReturning();
        long timeInFrightenStateLeft(long time);
        void setGhostEatenHandler( IGhostEatenHandler* value );
        void frighten(long time);
        void eaten(int score);
        void tabletEaten(long time, int tabletsEaten);
    };
}

#endif 
