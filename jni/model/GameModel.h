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
    class IntPosition;
    class IGameOverHandler;
 
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

        IGameOverHandler* gameOverHandler;

        public:
        GameModel(IGameOverHandler* gameOverHandler);
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
        int getScore();
        int getLives();
        int getEnergizerCount();
        GameState* getGameState();
        int getLevel();
        void setMotionDirection( MotionDirection* md );
        void checkGhostHit (IntPosition* pacManPosition, GhostModel* ghost);
    };
}

#endif 
