#include <android/log.h>  

#include "GameState.h"
  
#define DEBUG_TAG "NDK_GameState"
  
namespace game{
    GameState* GameState::LEVEL_INTRO       = new GameState ( 1000L, 1 );
    GameState* GameState::GAME              = new GameState ( -1, 1 );
    GameState* GameState::LEVEL_COMPLETED   = new GameState ( 3000L, 0 );
    GameState* GameState::GAME_OVER         = new GameState ( 3000L, 0 );
    
    GameState* GameState::POSSIBLE_GAME_STATES[4] = {
        GameState::LEVEL_INTRO,
        GameState::GAME,
        GameState::LEVEL_COMPLETED,
        GameState::GAME_OVER
    };

    GameState::GameState( long duration, int nextStateIndex ) {
        this->duration = duration;
        this->nextStateIndex = nextStateIndex;
    }
        
    long GameState::getDuration() {
        return duration;
    }
    
    GameState* GameState::getNextState() {
        return GameState::POSSIBLE_GAME_STATES[nextStateIndex];
    }
}