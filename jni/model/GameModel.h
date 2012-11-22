#ifndef GAME_MODEL
#define GAME_MODEL

#include "data/FieldSize.h"

namespace game{
    class PacManModel;
    class GhostModel;
    class PinkyModel;
    class InkeyModel;
    class ClydeModel;
    class MotionDirection;
    class GameState;
 
    class GameModel {
        private:
        PacManModel* pacManModel;
        GhostModel* blinkyModel;
        PinkyModel* pinkyModel;
        InkeyModel* inkeyModel;
        ClydeModel* clydeModel;
        
        PathArray fieldPath;
        PathArray tabletPath;
        PathArray energizerPath;
        
        int energizerCount;
        int score;
        long gameTime;
        int lives;
        int tabletsEaten;
        int ghostsEaten;
        int level;
        
        GameState* gameState;
        long enteredCurrentGameStateAt;
        
        void resetPersons();
        void checkGhostHit();

        public:
        GameModel();
        void step(long time);
        void resetLevel(long time);
        void resetGame(long time);
        PacManModel* getPacManModel();
        GhostModel* getBlinkyModel();
        GhostModel* getPinkyModel();
        GhostModel* getInkeyModel();
        GhostModel* getClydeModel();
        PathArray& getTabletPath();
        PathArray& getEnergizerPath();
        int getEnergizerCount();
        void setMotionDirection( MotionDirection* md );
    };
}

#endif 